#include <gui/main.h>
#include <fileio/fileio.h>

int main(int argc, char **argv) {
    int ret;
    fileio_init();
    ret = gui_main(argc, argv);
    fileio_exit();
    return ret;
}
