
class gsMD5
{
  private:
  public:
    gsMD5();
    ~gsMD5();

    char *get_md5(char *buf, int bufsz, char *md5buf);
    char *get_md5_from_file(char *filename, char *md5buf, int text=0);
};

