#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

//struct for store data from file
typedef struct dataset
{
  float input1;
  float input2;
  int desired_value;
}dataset;

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
  srand(time(NULL));
  for(i = 0; i < size; i++)
    {
      int random = rand() % size;
      swap(array, i, random);
    }
}

//calculate the mean square error
double mse(dataset *data, double w[], int input_size)
{ 
  double err_sum = 0;
  int i;
  for(i = 0; i<input_size; i++)
    {
       double sum = w[0] + w[1]*data[i].input1 + w[2]*data[i].input2;
       double error = data[i].desired_value - sum;
       err_sum += 0.5*error*error;
    }
  return sqrt(err_sum/input_size);
}

int main(int argc, char **argv)
{
  dataset data[2000]; // to store training data
  dataset test[2000]; // to store test data
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
  
  srand(time(NULL));
  double weights[3];
  //comments next lines out for random initial weights
  /*
    weights[0] = rand() % 100 - 50;
    weights[1] = rand() % 100 - 50;
    weights[2] = rand() % 100 - 50;
   */
  weights[0] = 1;
  weights[1] = 1;
  weights[2] = 1;
  double learning_rates = 0.001; // learning rate parameter 
  printf("Learning rate: %f\n", learning_rates);
  printf("Initial weights: %f %f %f \n", weights[0], weights[1], weights[2]);

  double diff;
  //FILE *fout = fopen("t1_0001.txt", "w");
  double mse1;
  int epoch = 0;
  printf("epoch: %d, MSE_trainining: %.9f, MSE_test: %.9f\n", 
	 epoch, mse(data, weights,2000), mse(test, weights, 2000));
  //comment next line out for changing the order of presentation
  //shuffle(data, 2000);
  do
    {
      mse1 = mse(data, weights, 2000);
      //double mse4 = mse(test, weights, 2000);
      //fprintf(fout, "%d %f %f\n", epoch, mse1, mse4);
      int i = 0;
 
      while(i < 2000) // one epoch for training
	{
	  double v = weights[0] + weights[1]*data[i].input1 + weights[2]*data[i].input2;
	  double error = data[i].desired_value - v;
	  weights[0] += learning_rates*error*1;
	  weights[1] += learning_rates*error*data[i].input1;
	  weights[2] += learning_rates*error*data[i].input2;
	  i++;
	}
      double mse2 = mse(data, weights, 2000);
      diff = mse1>mse2 ? mse1-mse2 : mse2-mse1;
      double mse3 = mse(test, weights, 2000);
      epoch++;
      printf("epoch: %d, MSE_trainining: %.9f, MSE_test: %.9f\n", epoch, mse1, mse3);
     
    }while(diff > mse1*1e-8); // the criterion for stopping learning
  //fclose(fout);
  printf("final weigths: %f %f %f\n", weights[0], weights[1], weights[2]);
  return 0;
}
