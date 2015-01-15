/* block-level tags for passing html blocks through the blender
 */
#ifndef _TAGS_D
#define _TAGS_D

struct kw {
    char *id;
    int  size;
    int  selfclose;
} ;


struct kw* mkd_search_tags(char *, int);
__declspec(dllexport) void mkd_prepare_tags();
__declspec(dllexport) void mkd_deallocate_tags();
__declspec(dllexport) void mkd_sort_tags();
__declspec(dllexport) void mkd_define_tag(char *, int);

#endif
