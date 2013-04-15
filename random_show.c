#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/* define a struct for a linked list */
struct node {
  char file[PATH_MAX];
  struct node *next;
};

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
      // printf("%s\n", path);

      /* 
       * check we have a regular file or a link
       * unfortunatly d_type won't work on some filesystems
       */
      
      /* you can't 'open' a file like you do with a directory */
      /*
      if(entry->d_type == DT_REG
          || entry->d_type == DT_LNK) {
      */
      if (!opendir(path)) {
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
      } else {
      /* if (entry->d_type == DT_DIR) { */
      /*
       * it opened so it is a directory,
       * scan it recursivly
       */
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
  /*
   * if no directory provided,
   * show help and terminate
   */
  if (! argv[1] || strcmp(argv[1], "-h") == 0) {
    fprintf (stderr, "Usage:\n%s directory\n",
        argv[0]);
    return 0;
  }

  /* create a linked list */
  /* create the root node */
  struct node *head;
  head = NULL;

  head = get_videos(argv[1], head);

  /* create a pointer to walk through the linked list */
  struct node *curr;
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

  if (nb_ep == 0) {
    printf("No video found in '%s'\n", argv[1]);
    return 0;
  }
  
  // printf("Found %d videos\n", nb_ep);


  /* get a random position in the linked list */
  srand(time(NULL));
  int random_number = rand()%(nb_ep)+1;

  /* take that random ep */
  curr = head;
  int i;
  for(i=1;i<random_number;i++) {
    curr = curr->next;
  }

  /* 
   * if a player was provided
   * create the command that should launch the file
   */
  if (argv[2]) {
    char command[PATH_MAX];
    sprintf(command, "%s \"%s\"", argv[2], curr->file);

    /* and do it ! */
    system(command);
  } else {
    printf("\"%s\"\n", curr->file);
  }

  return 0;
}
