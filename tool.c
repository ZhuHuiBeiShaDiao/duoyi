#include "tool.h"

char *string_add(const char *format,...)
{
	va_list arg_ptr;
	int len;
	char *res;
	int pipefd[2];

	va_start(arg_ptr,format);
#ifdef _WIN32
	if(_pipe(pipefd) == -1)
		return NULL;
	if(_dup2(pipefd[1],STDOUT_FILENO) == -1)
		return NULL;
	len=vprintf(format,arg_ptr);
	_dup2(STDOUT_FILENO,pipefd[1]);
#else
	if(pipe(pipefd) == -1)
		return NULL;
	len=vdprintf(pipefd[1],format,arg_ptr);
#endif
	close(pipefd[0]);
	close(pipefd[1]);

	va_end(arg_ptr);

	if(len < 0)
		return NULL;

	res=malloc(len+1);
	if(res == NULL)
		return NULL;
	va_start(arg_ptr,format);
	vsnprintf(res,len+1,format,arg_ptr);

	va_end(arg_ptr);

	return res;
}

char *url_encode(char *str)
{
	char *res;
	int len;
	int i,j;

	len=strlen(str)*3;
	res=malloc(len+1);
	bzero(res,len+1);

	for(i=0,j=0;str[i];++i,j+=3)
		sprintf(res+j,"%%%02x",(unsigned char)str[i]);

	return res;
}

unsigned char *base64_encode(unsigned char *str)
{
	long len;
	long str_len;
	unsigned char *res;
	int i,j;
	unsigned char *base64_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	str_len=strlen(str);
	if(str_len % 3 == 0)
		len=str_len/3*4;
	else
		len=(str_len/3+1)*4;

	res=malloc(sizeof(unsigned char)*len+1);
	res[len]='\0';

	for(i=0,j=0;i<len-2;j+=3,i+=4)
	{
		res[i]=base64_table[str[j]>>2];
		res[i+1]=base64_table[(str[j]&0x3)<<4 | (str[j+1]>>4)];
		res[i+2]=base64_table[(str[j+1]&0xf)<<2 | (str[j+2]>>6)];
		res[i+3]=base64_table[str[j+2]&0x3f];
	}

	switch(str_len % 3)
	{
		case 1:
			res[i-2]='=';
			res[i-1]='=';
			break;
		case 2:
			res[i-1]='=';
			break;
	}

	return res;
}

size_t curl_read_to_data(char *ptr,size_t size,size_t nmemb,DATA *data)
{
	if(data->data)
	{
		char *temp;
		int len;

		len=strlen(data->data);
		temp=malloc(sizeof(char)*len+1);
		snprintf(temp,len+1,"%s",data->data);
		free(data->data);

		data->data=malloc(sizeof(char)*len+nmemb+1);
		snprintf(data->data,len+nmemb+1,"%s%s",temp,ptr);
		free(temp);
	}
	else
	{
		data->data=malloc(sizeof(char)*nmemb+1);
		snprintf(data->data,nmemb+1,"%s",ptr);
	}
}