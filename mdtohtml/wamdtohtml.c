#ifdef _WIN32
#include <cstdio>
#include <sstream>
#endif

#include "wamdtohtml.h"

extern char *optarg;
extern int optind, opterr;

static void usage(void){
    fprintf(stderr, "usage: mdtohtml [stdin] [stdout]\n");
    fprintf(stderr, " -%c: %s\n",OPT_WITH_HTML5_TAGS , "With HTML5 tags");    
    fprintf(stderr, " -%c: %s\n",OPT_GITHUB_FLAVOURED , "GitHub Flavoured");  
    fprintf(stderr, " -%c number : %s\n",OPT_FLAGS , "Flags");     
    exit(1);
}

#pragma mark -

int main(int argc, char *argv[])
{
    int with_html5_tags = 0;
    int github_flavoured = 0; 
    mkd_flag_t flags = 0;
    
    if(argc == 1) usage();
    
    int ch;
    
    while ((ch = getopt(argc, argv, OPT_LIST)) != -1){
        
        switch (ch){
            case OPT_WITH_HTML5_TAGS:
                with_html5_tags = 1;
                break;
                
            case OPT_GITHUB_FLAVOURED:
                github_flavoured = 1;
                break;

            case OPT_FLAGS:
                flags = atoi(optarg);
                break;
                
            default:
                usage();
        }
    } 
    
    if(with_html5_tags){
        mkd_with_html5_tags();
    }
    
    unsigned int size = BUFFER_SIZE;
    
    char *buf = (char *)calloc(size, sizeof(char));
    
    if(!buf){
        exit(1);
    }
    
    unsigned int ret, i = 0;
        
    while((ret = fread(&buf[i], sizeof(char), BUFFER_SIZE, stdin)) > 0){
        i += ret;
        size += BUFFER_SIZE;
        buf = (char *)realloc(buf, size);
        if(!buf){
            exit(1);
        }
    } 

#ifdef _WIN32    
	Document *doc;
    std::ostringstream outstream;
#else
    MMIOT *doc;
#endif    
    
    if(github_flavoured){
        doc = gfm_string((const char *)&buf[0], (int)i, flags);	
    }else{
        doc = mkd_string((const char *)&buf[0], i, flags);	
    }
    
    if(doc){
        if(mkd_compile(doc, flags)){
#ifdef _WIN32    
            mkd_generatecss(doc, outstream);
            mkd_generatetoc(doc, outstream);
            mkd_generatehtml(doc, outstream);
#else
            mkd_generatecss(doc, stdout);
            mkd_generatetoc(doc, stdout);
            mkd_generatehtml(doc, stdout);            
#endif             
        }
        mkd_cleanup(doc);
    }
#ifdef _WIN32    
    mkd_deallocate_tags();  
    std::string dst = outstream.str();
    printf("%s", dst.c_str());        
#endif    

    free(buf);

    return 0;
}
