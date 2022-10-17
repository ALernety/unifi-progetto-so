#include "REGISTRO.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#define DEFAULT_PROTOCOL 0

int creaPipeConNome(char *nome) {
  int pipeFd;
  unlink(nome);
  mknod(nome, S_IFIFO | 0660, 0);
  do {
    pipeFd = open(nome, O_WRONLY);
    if (pipeFd == -1) {

      sleep(1);
    }
  } while (pipeFd == -1);
  return pipeFd;
}
