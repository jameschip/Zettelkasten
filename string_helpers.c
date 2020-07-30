
void remove_trailing_white( char * c ) {
    int index, i;
    index = -1;
    i = 0;
    
    while( c[i] != '\0')
    {
        if (c[i] == '\n') {
            c[i + 1] = '\0';
            return;
        }

        if(c[i] != ' ' && c[i] != '\t')
        {
            index= i;
        }
        i++;
    }
    
    c[index + 1] = '\0';
}
