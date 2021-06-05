#if !defined(MAPFILE_H_)
#define MAPFILE_H_

char *allocate_base(char *filepath, int *fd);
void umap_file(char *base, size_t filesize);

extern size_t filesize;

#endif // MAPFILE_H_
