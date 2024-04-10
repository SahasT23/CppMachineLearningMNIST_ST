 #include "data_handler.hpp"
 
// implementing methods 
data_handler::data_handler()  // constructor
{
  data_array = new std::vector<data *>;
  test_data = new std::vector<data *>;
  training_data = new std::vector<data *>;
  validation_data = new std::vector<data *>;
}
data_handler::~data_handler() // destructor
{
  // Free dynamically allocated stuff
}

void data_handler::read_feature_vector(std::string path) // since data file and label file are separate, we read them separately
{
  uint32_t header[4]; // |MAGIC|NUM IMAGES|ROWSIZE|COLSIZE order of the 4 elements
  unsigned char bytes[4]; // All 32 bits can be read in
  FILE *f = fopen(path.c_str(), "r");
  if(f) // if the file pointer is not null
  {
    for(int i = 0; i < 4; i++)
    {
      if(fread(bytes, sizeof(bytes), 1, f))
      {
        header[i] = convert_to_little_endian(bytes);
      }
    }
    printf("Done getting input file header.\n");
    int image_size = header[2]*header[3]; // iterating through the number of images
    for(int i = 0; i < header[1]; i++ )
    {
      data *d = new data(); // iterating over the next image size elements and initialising data variable
      uint8_t element[1]; // one element array of size 8 bits 
      for(int j = 0; j < image_size; j++)
      {
        if(fread(element, sizeof(element), 1, f))
        {
          d->append_to_feature_vector(element[0]); // error handling if-else statement
        } else 
        {
          printf("error reading from the file.\n");
          exit(1);
        }
      } // at the end of this loop, the entire data array will be collected and filled with the feature vectors
      data_array->push_back(d); // data array is storing a pointer to that value
    }
    printf("Successfully read and stored %lu feature vectors.\n", data_array->size());
  } else // no file pointer 
  {
    printf("could not find file.\n");
    exit(1);
  }
}
void data_handler::read_feature_labels(std::string path)
{
    uint32_t header[2]; // |MAGIC|NUM IMAGES| order of the 2 elements
  unsigned char bytes[2]; // All 32 bits can be read in
  FILE *f = fopen(path.c_str(), "r");
  if(f) // if the file pointer is not null
  {
    for(int i = 0; i < 2; i++)
    {
      if(fread(bytes, sizeof(bytes), 1, f))
      {
        header[i] = convert_to_little_endian(bytes);
      }
    }
    printf("Done getting label file header.\n");
    for(int i = 0; i < header[1]; i++ )
    {
      uint8_t element[1]; // one element array of size 8 bits 
        if(fread(element, sizeof(element), 1, f))
        {
          data_array->at(i)->set_label(element[0]);
        } else 
        {
          printf("error reading from the file.\n");
          exit(1);
        }
      // at the end of this loop, the entire data array will be collected and filled with the feature vectors
    }
    printf("Successfully read and stored labels.\n", data_array->size());
  } else // no file pointer 
  {
    printf("could not find file.\n");
    exit(1);
  }
}

void data_handler::split_data()
{
  std::unordered_set<int> used_indexes;
  int train_size = data_array->size() * TRAIN_SET_PERCENT;
  int test_size = data_array->size() * TEST_SET_PERCENT;
  int valid_size = data_array->size() * VALIDATION_TEST_PERCENT;

  // Training data
  int count = 0;
  while(count < train_size)
  {
    int rand_index = rand() % data_array->size(); // 0 & data_array->size() -1
    if(used_indexes.find(rand_index) == used_indexes.end())
    {
      training_data->push_back(data_array->at(rand_index));// data is read into the data array and passing pointers to the data
      used_indexes.insert(rand_index);
      count++;
    }
  }

  // Testing data
  int count = 0;
  while(count < test_size)
  {
    int rand_index = rand() % data_array->size(); // 0 & data_array->size() -1
    if(used_indexes.find(rand_index) == used_indexes.end())
    {
      test_data->push_back(data_array->at(rand_index));// data is read into the data array and passing pointers to the data
      used_indexes.insert(rand_index);
      count++;
    }
  }

  // Validation data
  int count = 0;
  while(count < valid_size)
  {
    int rand_index = rand() % data_array->size(); // 0 & data_array->size() -1
    if(used_indexes.find(rand_index) == used_indexes.end())
    {
      validation_data->push_back(data_array->at(rand_index));// data is read into the data array and passing pointers to the data
      used_indexes.insert(rand_index);
      count++;
    }
  }

  printf("Training Data Size: %lu.\n", training_data->size());
  printf("Testing Data Size: %lu.\n", test_data->size());
  printf("Validation Data Size: %lu.\n", validation_data->size());


}
void data_handler::count_classes();

uint32_t data_handler::convert_to_little_endian(const unsigned char* bytes);

std::vector<data *> * data_handler::get_training_data();
std::vector<data *> * data_handler::get_test_data();
std::vector<data *> * data_handler::get_validation_data();