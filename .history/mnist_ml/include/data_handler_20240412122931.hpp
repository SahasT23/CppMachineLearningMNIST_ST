/*
header file implementing the logic to read, split the data, count the number of unique classes
and to pass around the test and validation data 
*/

#ifndef __DATA_HANDLER_H
#define __DATA_HANDLER_H

#include <fstream>  // to read file.io
#include "stdint.h" 
#include "data.hpp" // to read uint8 data type
#include <vector>
#include <string> // strings make it easier, don't have to worry about construct pointers
#include <map> // to map a class label to enum value, quick access
#include <unordered_set> // keeps track of train data, test data and validation data

class data_handler
{
  std::vector<data *> *data_array; // all of the types of data (pre-split)
  std::vector<data *> *training_data;
  std::vector<data *> *test_data;
  std::vector<data *> *validation_data;

  int num_classes; // counting the number of classes that we have
  int feature_vector_size;
  std::map<uint8_t, int> class_map;

  /*
  Training set encompasses most of the data
  Testing dataset is to check how good our model is
  Need to feed forward and back propagate until the performance matches the pre-validated set
  Uses completely new data to see how good our model is
  */
  const double TRAIN_SET_PERCENT = 0.75;
  const double TEST_SET_PERCENT = 0.20;
  const double VALIDATION_TEST_PERCENT = 0.05;

  // implementing methods 
  public:
  data_handler(); // constructor
  ~data_handler(); // destructor

  void read_feature_vector(std::string path); // since data file and label file are separate, we read them separately
  void read_feature_labels(std::string path);
  void split_data();
  void count_classes();

  uint32_t convert_to_little_endian(const unsigned char* bytes);

  std::vector<data *> * get_training_data();
  std::vector<data *> * get_test_data();
  std::vector<data *> * get_validation_data();

};

#endif