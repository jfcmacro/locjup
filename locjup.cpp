#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include "locjup.h"

// I hate this global variables are evil
static pid_t jupchild;

static void show_help(const char* progname) {
  std::cout << progname
	    << " [-h|--help] show this output"
	    << std::endl
	    << progname
	    << " [-v|--version] show current version"
	    << std::endl;
  exit(EXIT_SUCCESS);
}

static void show_version(const char* progname) {
  std::cout << progname
	    << " version "
	    << LOCJUP_VERSION_MAJOR
	    << "."
	    << LOCJUP_VERSION_MINOR << std::endl;
  exit(EXIT_SUCCESS);
}

static void handler(int signo) {
  kill(jupchild, signo);
}

int
main(int argc, char *argv[]) {
  int c;
  int option_index = 0;
  static struct option long_options[] = {
    {"help", 0, 0, 0},
    {"version", 0, 0, 0},
    {0, 0, 0, 0}};

  char *dirprogname = ::strdup(argv[0]);
  char *progname = ::basename(dirprogname);

  while ((c = getopt_long(argc, argv, "hv", long_options, &option_index)) != -1) {
    switch(c) {
    case 0:
      switch(option_index) {
      case 0:
	show_help(progname);
	break;
      case 1:
	show_version(progname);
	break;
      default:
	show_help(progname);
	break;
      }
      break;
    case 'h':
      show_help(progname);
      break;
    case 'v':
      show_version(progname);
      break;
    default:
      break;
    }
  }

  if ((jupchild = ::fork()) == 0) {
    execlp("jupyter", "jupyter", "notebook", "--notebook-dir=\".\"", nullptr);
    exit(127);
  }
  else {
    struct sigaction sa_old;
    struct sigaction sa_new;

    sa_new.sa_handler = handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    ::sigaction(SIGINT, &sa_new, &sa_old);
    ::sleep(5);
    if (::fork() == 0) {
      ::execlp("xdg-open", "xdg-open", "https://localhost:8888", nullptr);
      ::exit(128);
    }
    else {
      int status;
      ::waitpid(jupchild, &status, 0);
    }

    ::sigaction(SIGINT, &sa_old, nullptr);
  }

  return EXIT_SUCCESS;
}
