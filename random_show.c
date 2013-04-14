#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/* define a struct for a linked list */
typedef struct node {
  char file[PATH_MAX];
  struct node *next;
} node;

struct node *get_videos(char *dir_name, struct node *head) {
  // printf("%s:\n", dir_name);
  DIR * dir;
  struct dirent* entry;

  /* open directory */
  dir = opendir(dir_name);

  /* check opening was ok */
  if (! dir) {
    fprintf (stderr, "Cannot open directory '%s': %s\n",
        dir_name, strerror (errno));
    exit (EXIT_FAILURE);
  }

  if (dir != NULL) {
    while((entry = readdir(dir)) != NULL) {
      /* we don't want to parse "." or ".." */
      if (strcmp(entry->d_name, "..") == 0
          || strcmp(entry->d_name, ".") == 0)
        continue;
      
      /* get the full path */
      char path[PATH_MAX];
      sprintf (path,
          "%s/%s", dir_name, entry->d_name);

      /* check we have a regular file or a link */
      if(entry->d_type == DT_REG
          || entry->d_type == DT_LNK) {

        /* get extension */
        const char * ext = strrchr(path, '.');

        /* if we have an extension, the fun can begin */
        if (ext) {
          /* does the extension match a video file ? */
          if (strcmp(ext, ".avi") == 0
              || strcmp(ext, ".mpg") == 0
              || strcmp(ext, ".mkv") == 0) {
            /* allocate size for a new node */
            struct node *new = malloc( sizeof(struct node) );
            /* copy the string value */
            strcpy(new->file, path);
            new->next = head;
            head = new;
          }
        }
      }
  
      /* if there is a subdirectory scan it recursivly */
      if (entry->d_type == DT_DIR) {
        head = get_videos(path, head);
      }
    }
  }

  /* close directory */
  if (closedir(dir)) {
    fprintf (stderr, "Could not close '%s': %s\n",
        entry->d_name, strerror (errno));
    exit (EXIT_FAILURE);
  }

  /* return the new head */
  return head;
}

int main (int argc, char *argv[]) {
  /* create a linked list */
  /* create the root node */
  node *head;
  head = NULL;

  head = get_videos(argv[1], head);

  /* create a pointer to walk through the linked list */
  node *curr;
  curr = head;

  /*
   * how many ep are there ?
   * yes it has to loop through all the linked list
   * which is probably not the best thing
   */
  int nb_ep = 0;
  while(curr) {
    nb_ep++;
    curr = curr->next;
  }
  printf("nb ep: %d\n", nb_ep);


  /* get a random position in the linked list */
  srand(time(NULL));
  int random_number = rand()%(nb_ep)+1;

  /* take that random ep */
  curr = head;
  int i;
  for(i=1;i<random_number;i++) {
    curr = curr->next;
  }

  /* create the command that should launch the file */
  char command[PATH_MAX];
  sprintf(command, "%s '%s'", argv[2], curr->file);

  /* and do it ! */
  system(command);

  return 0;
}
