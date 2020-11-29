/*
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"



int main(int argc, char ** argv) {
  //YOUR CODE GOES HERE
  return EXIT_SUCCESS;
}
*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"

int win_hand(deck_t ** deck_array,int n_hands){
  int w[n_hands+1];
  for(int u=0;u<n_hands+1;u++) w[u]=0;
  int v=0;
  for(int i =0;i<n_hands-1;i++){
    for (int j=i+1;j<n_hands;j++){
      // print_hand(deck_array[i]);
      //            print_hand(deck_array[j]);
      // printf("\n");
      v=compare_hands(deck_array[i],deck_array[j]);
      if(v>0) w[i]++;
      else if (v<0) w[j]++;
      else w[n_hands]++;
    }
  }
  unsigned largest= 0;
  for(int x=0;x<n_hands+1;x++){
    if(w[x] > w[largest])largest=x;
  }
  int count=0;
  if(w[n_hands]>0){
    for(int x=0;x<n_hands+1;x++){
      if(w[x] == w[largest]) count++;
    }
  }
  if(count>1) return n_hands;
  return  largest;
}

int main(int argc, char ** argv) {
  //check arguments
  if (argc < 2 ){
    fprintf(stderr, "not enough number of arguments\n");
    return EXIT_FAILURE ;
  }
  //Open the input file and read the hands in it
  FILE* f=fopen(argv[1],"r");
  if(f == NULL) {
    fprintf(stderr, "not valid  input file\n");
    return EXIT_FAILURE ;
  }
  deck_t ** deck_array=NULL;
  size_t n_hands =0 ;
  future_cards_t * fc=malloc(sizeof(*fc));
  fc->decks=NULL;
  fc->n_decks=0;
  deck_array=read_input(f,&n_hands,fc);
  //Create a deck with the remaining cards
  deck_t* sh=build_remaining_deck(deck_array, n_hands);
  int win_array[n_hands+1];
  for(int u=0;u<n_hands+1;u++) win_array[u]=0;
  // Do each Monte Carlo trial (repeat num_trials times)
  int num_trials = 10000;
  if (argc ==3) num_trials=atoi(argv[2]);
  for(int i=0;i<num_trials;i++){
    //Shuffle the deck of remaining cards
    shuffle(sh);
    //Assign unknown cards from the shuffled deck
    future_cards_from_deck(sh,fc);
    //Use compare_hands (from Course 3)
    int c =win_hand(deck_array,n_hands);
    win_array[c]++;
  }
  //you just need to print your results
  for(size_t j=0;j<n_hands;j++){
    printf("Hand %zu won %u / %u times (%.2f%%)\n",j,win_array[j],num_trials,(((float)win_array[j])/num_trials)*100);
  }
  printf( "And there were %u ties\n",win_array[n_hands]);
  //free any memory you allocated
  for(int x=0;x<n_hands;x++){
    free_deck(deck_array[x]);
  }
  free(deck_array);
  for(int o=fc->n_decks-1 ;o>=0;o--){
    if(fc->decks[o].n_cards != 0) free(fc->decks[o].cards);
  }
  free(fc->decks);
  free(fc);
  free_deck(sh);

  if(fclose(f) != 0){
    fprintf(stderr, "close file");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"



void free_input(deck_t ** input, size_t num){
  for (size_t i=0; i<num; i++){
    free_deck(input[i]);
  }
  free(input);
}


int main(int argc, char ** argv) {
  if (argc > 3 || argc < 2) {
    fprintf(stderr, "Usage: enter hand and number of trials\n");
    return EXIT_FAILURE;
  }

  // if trial number not entered you should use 10000 as default value
  unsigned trials = 10000;
  if (argc==3){
    trials = atoi(argv[2]);
  }

  FILE * f = fopen(argv[1], "r");
  if (f==NULL){
    fprintf(stderr, "File not found\n");
    return EXIT_FAILURE;
  }

  future_cards_t * fc = malloc(sizeof(* fc));
  fc->decks = NULL;
  fc->n_decks = 0;
  size_t n_hands = 0;
  // read the input also updating n_hands
  deck_t ** input = read_input(f, &n_hands, fc);
  deck_t * remaining_cards = build_remaining_deck(input, n_hands);


  // PRINT FOR CHECKING
  // for (int i=0; i<n_hands; i++){
  //print_hand(input[i]);
  // printf("\n");
  //}
  //print_hand(remaining_cards);
  // printf("\n");
  // printf("number of trials to do %u\n",trials);

  // create w_array and initialize (last slot should count ties)
  unsigned w_array[n_hands+1];
  for (int i=0; i<n_hands+1; i++){
    w_array[i]=0;
    // printf("w_array index[%d] = %d\n", i, w_array[i]);
  }

  for (int i = 0; i<trials; i++){
    shuffle(remaining_cards);
    future_cards_from_deck(remaining_cards, fc);

    // find out which hand to score, by default its hand zero so if first wins, dont shift at all
    int inc_array_at=0;
    int index=0;
    for( int x = 1; x<n_hands; x++){
      if (compare_hands(input[index], input[x])==0) inc_array_at = n_hands;
      if (compare_hands(input[index], input[x])<0) {
	inc_array_at = x;
	index=x;
      }
    }
    w_array[inc_array_at]++;
  }


  /* for (int i=0; i<n_hands+1; i++){ */
  /*   // w_array[i]=0; */
  /*    printf("w_array index[%d] = %d\n", i, w_array[i]); */
  /* } */

  for (size_t i=0; i<n_hands; i++){
    printf("Hand %zu won %u / %u times (%.2f%%)\n", i, w_array[i], trials, (double)w_array[i]/(double)trials*100);
  }
  printf("And there were %u ties\n", w_array[n_hands]);

  // free and close files

  if (fclose(f)!=0){
    fprintf(stderr, "Cannot close file\n");
    return EXIT_FAILURE;
  }

  free_input(input, n_hands);
  free_deck(remaining_cards);
  for (size_t i=0; i<fc->n_decks; i++) free(fc->decks[i].cards);
  free(fc->decks);
  free(fc);


  return EXIT_SUCCESS;
}
