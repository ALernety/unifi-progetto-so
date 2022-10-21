/**
 * This function is used to get the parent directory of some depth
 * related to the file or another directory.
 * @param parent_dir String where to save the value of parent_dir.
 * @param path Relative path from which should be taken parent_dir.
 * @param parent_depth Depth of parent directory (if is 0 will return file).
 * @return Absolute path to the parent directory.
 */
char *parent_dir(char *parent_dir, const char *path, unsigned int parent_depth);