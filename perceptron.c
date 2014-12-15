#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//struct for storing data from file
typedef struct dataset
{
  float input1;
  float input2;
  int desired_value;
}dataset;

//step function for perceptron
int step_function(float a)
{
  if(a <= 0)
    return -1;
  else
    return 1;
}

//swap two items in array
void swap(dataset array[], int i, int j)
{
  dataset temp;
  temp.input1 = array[i].input1;
  temp.input2 = array[i].input2;
  temp.desired_value = array[i].desired_value;
  array[i].input1 = array[j].input1;
  array[i].input2 = array[j].input2;
  array[i].desired_value = array[j].desired_value;
  array[j].input1 = temp.input1;
  array[j].input2 = temp.input2;
  array[j].desired_value = temp.desired_value;
}

//shuffle the array
void shuffle(dataset array[], int size)
{
  int i;
  //srand(time(NULL));
  for(i = 0; i < size; i++)
    {
      int random = rand() % size;
      swap(array, i, random);
    }
}


int main(int argc, char **argv)
{
  dataset data[2000];
  dataset test[2000];
  if(argc != 5)
    {
      printf("Usage: program training_class1 training_class2 test_class1 test_class2\n");
      exit(1);
    }

  //open training data file and test data file
  FILE *fp1, *fp2, *fp3, *fp4;
  if((fp1 = fopen(argv[1], "r")) == NULL)
    {
      printf("cannot open %s\n", argv[1]);
      exit(1);
    }
  if((fp2 = fopen(argv[2], "r")) == NULL)
    {
      printf("cannot open %s\n", argv[2]);
      exit(1);
    }
  if((fp3 = fopen(argv[3], "r")) == NULL)
    {
      printf("cannot open %s\n", argv[3]);
      exit(1);
    }
  if((fp4 = fopen(argv[4], "r")) == NULL)
    {
      printf("cannot open %s\n", argv[4]);
      exit(1);
    }

  //get data from file and store to array
  int i = 0;
  int num;
  while(i < 2000)
   {
     if(i < 1000)
       {
	 fscanf(fp1, "%d %f %f", &num, &data[i].input1, &data[i].input2);
	 data[i].desired_value = 1;
	 fscanf(fp3, "%d %f %f", &num, &test[i].input1, &test[i].input2);
	 test[i].desired_value = 1;
       }
     else
       {
	 fscanf(fp2, "%d %f %f", &num, &data[i].input1, &data[i].input2);
	 data[i].desired_value = -1;
	 fscanf(fp4, "%d %f %f", &num, &test[i].input1, &test[i].input2);
	 test[i].desired_value = -1;
       }
     i++;
   }

  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);
  float weights[3];
  //comment next line out for random initial weights and changing order of presentation
  //srand(time(NULL));
  
  //comment next lines out for random initial weights 
  /*
  weights[0] = (rand()%100 - 50)/(float)100;
  weights[1] = (rand()%100 - 50)/(float)100;
  weights[2] = (rand()%100 - 50)/(float)100;
  */
  
  weights[0] = 1;
  weights[1] = 1;
  weights[2] = 1;
  
  int adjusted_items;
  float learning_rates = 0.0001; //learning rate parameter
  int epoch = 1; //epoch number
  int test_items;
  printf("Learning rate: %f\n", learning_rates);
  printf("Initial weights: %f %f %f\n", weights[0], weights[1], weights[2]);
  //FILE *fout = fopen("p_001.txt", "w");
  //comment next line out for changing the order of presentation
  //shuffle(data, 2000);
  do{
    i = 0;
    adjusted_items = 0;
    while(i < 2000)   //one epoch for learning
      {
	float v = weights[0] + weights[1]*data[i].input1 + weights[2]*data[i].input2;
	int error = data[i].desired_value - step_function(v);
	if(error != 0)  
	  {
	    weights[0] += learning_rates*error*1;
	    weights[1] += learning_rates*error*data[i].input1;
	    weights[2] += learning_rates*error*data[i].input2;
	    adjusted_items++;
	  }
	i++;
      }    
    test_items = 0;
    i = 0;
    while(i < 2000)
      {
	float v = weights[0] + weights[1]*test[i].input1 + weights[2]*test[i].input2;
	int error = test[i].desired_value - step_function(v);
	if(error != 0)  
	  {
	    test_items++;
	  }
	i++;
      }    
    printf("Epoch: %d AdjustedItemsForTraining: %d TestItemsWithoutTarget: %d\n", 
	   epoch, adjusted_items,test_items);
    //fprintf(fout, " %d  %d %d\n", epoch, adjusted_items,test_items);
    epoch++;
  }while(adjusted_items != 0);  //criterion for stopping learning
  printf("Final weights: %f %f %f\n", weights[0], weights[1], weights[2]);
  //fclose(fout);
  return 0;
}
