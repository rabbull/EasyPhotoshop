#include <gui/main.h>
#include <fileio/fileio.h>

int main(int argc, char **argv) {
    fileio_init();
    return gui_main(argc, argv);
}
