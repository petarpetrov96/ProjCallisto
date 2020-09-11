void cut(char* org,int start,int len,char* where) {
    memcpy(where,org+start,len);
    /*char* ret=new char[len];
    for(int i=start;i<start+len;i++)
        ret[i-start]=org[i];
    ret[len]='\0';
    return ret;*/
}
void append(char* org, char* str, int start, int len) {
    memcpy(org+start,str,len);
    /*for(int i=start;i<start+len;i++)
        org[i]=str[i-start];*/
}
void floatToString(float f, char* t) {
    memcpy(t,&f,sizeof(f));
}
float stringToFloat(char s[4]) {
    float f;
    memcpy(&f,s,sizeof(float));
    return f;
}
