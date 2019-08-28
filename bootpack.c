void io_hlt(void);

void MyOSMain(void)
{

fin:
    io_hlt();
    goto fin;
}
