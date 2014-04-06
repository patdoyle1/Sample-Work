#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

#include "image.h"
#include "priority_queue.h"

// ===================================================================================================

// distance field method functions
double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image);

// visualization style helper functions
Color Rainbow(double distance, double max_distance);
Color GreyBands(double distance, double max_distance, int num_bands);

// ===================================================================================================

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " input.ppm output.ppm distance_field_method visualization_style" << std::endl;
    exit(1);
  }

  // open the input image
  Image<Color> input;
  if (!input.Load(argv[1])) {
    std::cerr << "ERROR: Cannot open input file: " << argv[1] << std::endl;
    exit(1);
  }

  // a place to write the distance values
  Image<DistancePixel> distance_image;
  distance_image.Allocate(input.Width(),input.Height());

  // calculate the distance field (each function returns the maximum distance value)
  double max_distance = 0;
  if (std::string(argv[3]) == std::string("naive_method")) {
    max_distance = NaiveDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("improved_method")) {
    max_distance = ImprovedDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_map")) {
    max_distance = FastMarchingMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_hash_table")) {
    // EXTRA CREDIT: implement FastMarchingMethod with a hash table
  } else {
    std::cerr << "ERROR: Unknown distance field method: " << argv[3] << std::endl;
    exit(1);
  }

  // convert distance values to a visualization
  Image<Color> output;
  output.Allocate(input.Width(),input.Height());
  for (int i = 0; i < input.Width(); i++) {
    for (int j = 0; j < input.Height(); j++) {
      double v = distance_image.GetPixel(i,j).getValue();
      if (std::string(argv[4]) == std::string("greyscale")) {
  output.SetPixel(i,j,GreyBands(v,max_distance*1.01,1));
      } else if (std::string(argv[4]) == std::string("grey_bands")) {
  output.SetPixel(i,j,GreyBands(v,max_distance,4));
      } else if (std::string(argv[4]) == std::string("rainbow")) {
  output.SetPixel(i,j,Rainbow(v,max_distance));
      } else {
  // EXTRA CREDIT: create other visualizations 
  std::cerr << "ERROR: Unknown visualization style" << std::endl;
  exit(0);
      }
    }
  }
  // save output
  if (!output.Save(argv[2])) {
    std::cerr << "ERROR: Cannot save to output file: " << argv[2] << std::endl;
    exit(1);
  }
}

// ===================================================================================================

double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  int w = input.Width();
  int h = input.Height();
  // return the maximum distance value
  double answer = 0;
  // loop over the pixels in the input image
  for (int i = 0; i < w; i++)  {
    for (int j = 0; j < h; j++) {
      double closest = -1;      
      // loop over all other pixels in the input image
      for (int i2 = 0; i2 < w; i2++)  {
  for (int j2 = 0; j2 < h; j2++) {
    const Color& c = input.GetPixel(i2,j2);      
    // skip all pixels that are not black
    if (!c.isBlack()) continue;
    // calculate the distance between the two pixels
    double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));
    // store the closest distance to a black pixel
    if (closest < 0 || distance < closest) {
      closest = distance;
    }
  }
      }
      assert (closest >= 0);
      answer = std::max(answer,closest);
      // save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
}


double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
std::vector<std::pair<int, int> > blackPixels;
  //find all black pixels, then look through found points and compare them to the black pixels
  // IMPLEMENT THIS FUNCTION
  //
  // a small improvement on the NaiveDistanceFieldMethod
  //
   int w = input.Width();
  int h = input.Height();
  // return the maximum distance value
  double answer = 0;
   for (int x = 0; x < w; x++) 
      {
       for (int y = 0; y < h; y++) 
       {
          const Color& d = input.GetPixel(x,y);      
          // skip all pixels that are not bladk
          if (d.isBlack())
          {
            blackPixels.push_back(std::make_pair(x,y));

          }          
        }
      }
  // loop over the pixels in the input image
  for (int i = 0; i < w; i++)  {
    for (int j = 0; j < h; j++) {
      double closest = -1;      
      // loop over all other pixels in the input image
       for (int k = 0; k < blackPixels.size(); k++) 
       {
          // calculate the distance between the two pixels
          int i2 = blackPixels[k].first;
          int j2 = blackPixels[k].second;
          double distance = sqrt((i-blackPixels[k].first)*(i-blackPixels[k].first) + (j-blackPixels[k].second)*(j-blackPixels[k].second));
          // store the closest distance to a black pixel
          if (closest < 0 || distance < closest) {
            closest = distance;
          }
        }
      
      assert (closest >= 0);
      answer = std::max(answer,closest);
      // save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
  
}

std::vector<std::pair<int, int> > getNearbyPixels(int x, int y, Image<DistancePixel> &distance_image){
  std::vector<std::pair<int, int> > nearbyPixels;
  if(x+1 < distance_image.Width())
  {
    if(y+1 < distance_image.Height())
    {
      if(distance_image.GetPixel(x+1,y+1).isFinalValue() == false)
        nearbyPixels.push_back(std::make_pair(x+1,y+1));

    }
  }
  if(x+1 < distance_image.Width())
  {
      if(distance_image.GetPixel(x+1,y).isFinalValue() == false)
        nearbyPixels.push_back(std::make_pair(x+1,y));

    
  }

    if(y+1 < distance_image.Height())
    {
      if(distance_image.GetPixel(x,y+1).isFinalValue() == false)
        nearbyPixels.push_back(std::make_pair(x,y+1));

    }
  
  if(x-1 >= 0)
  {
    if(y+1 < distance_image.Height())
    {
      if(distance_image.GetPixel(x-1,y+1).isFinalValue() == false)
        nearbyPixels.push_back(std::make_pair(x-1,y+1));
    }

  }
  if(x-1 >=0)
  {
      if(distance_image.GetPixel(x-1,y).isFinalValue() == false)
        nearbyPixels.push_back(std::make_pair(x-1,y));
    
  }
  if(x-1 >=0)
  {
    if(y-1 >=0)
    {
      if(distance_image.GetPixel(x-1,y-1).isFinalValue() == false)
        nearbyPixels.push_back(std::make_pair(x-1,y-1));
    }

  }
  if(x+1 < distance_image.Width())
  {
    if(y-1 >= 0)
    {
      if(distance_image.GetPixel(x+1,y-1).isFinalValue() == false)
        nearbyPixels.push_back(std::make_pair(x+1,y-1));
    }
  }
      if(y-1 >=0)
      {
        if(distance_image.GetPixel(x,y-1).isFinalValue() == false)
          nearbyPixels.push_back(std::make_pair(x,y-1));
      }
  return nearbyPixels;
}

double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  int w = input.Width();
  int h = input.Height();
  std::vector<std::pair<int, int> > blackPixels;
  for (int x = 0; x < w; x++) 
    {
       for (int y = 0; y < h; y++) 
       {
          const Color& d = input.GetPixel(x,y);
          DistancePixel& beefy = distance_image.GetPixel(x,y);   
          beefy.setX(x);
          beefy.setY(y);
          // skip all pixels that are not bladk
          if (d.isBlack())
          {
            blackPixels.push_back(std::make_pair(x,y));
            beefy.setValue(0);
            beefy.makeFinalValue();
          }
          else
            beefy.setValue(1000000); // ome million motherfucker          
        }
    }
  double answer = 0;
  DistancePixel_PriorityQueue theQueue;
  for(int x = 0; x < blackPixels.size(); x++){
    int b = blackPixels[x].first;
    int j = blackPixels[x].second;
    DistancePixel& tempPixel = distance_image.GetPixel(b,j);
    std::vector<std::pair<int, int> > nearbyPixels = getNearbyPixels(b,j, distance_image);
    for (int v = 0; v < nearbyPixels.size(); v++)
    {
      int xVal = nearbyPixels[v].first;
      int yVal = nearbyPixels[v].second;
      DistancePixel& somePixelWorkin = distance_image.GetPixel(xVal,yVal);
      double distance = tempPixel.getValue() + (sqrt((b-xVal)*(b-xVal) + (j-yVal)*(j-yVal)));
      if(somePixelWorkin.getValue() > distance){
        somePixelWorkin.setValue(distance);
        if(!theQueue.in_heap(&somePixelWorkin))
          theQueue.push(&somePixelWorkin);
        else
          theQueue.update_position(&somePixelWorkin);
      }
    }
  }
  while(theQueue.size() > 0){
    const DistancePixel* aPixel = theQueue.top();
    int b = aPixel->getX();
    int j = aPixel->getY();
    DistancePixel& anotherPixel = distance_image.GetPixel(b, j);
    anotherPixel.makeFinalValue();
    std::vector<std::pair<int, int> > nearbyPixels = getNearbyPixels(b,j, distance_image);
    for (int v = 0; v < nearbyPixels.size(); v++)
    {
      int xVal = nearbyPixels[v].first;
      int yVal = nearbyPixels[v].second;
      DistancePixel& somePixelWorkin = distance_image.GetPixel(xVal,yVal);
      double distance = anotherPixel.getValue() + (sqrt((b-xVal)*(b-xVal) + (j-yVal)*(j-yVal)));
      if(somePixelWorkin.getValue() > distance){
        somePixelWorkin.setValue(distance);
        if(!theQueue.in_heap(&somePixelWorkin))
          theQueue.push(&somePixelWorkin);
        else
          theQueue.update_position(&somePixelWorkin);
      }
    }
    answer = std::max(answer, anotherPixel.getValue());
    theQueue.pop();
  }
  return answer;
}
  
// ===================================================================================================

Color Rainbow(double distance, double max_distance) {
  Color answer;
  if (distance < 0.001) {
    // black
    answer.r = 0; answer.g = 0; answer.b = 0;
  } else if (distance < 0.2*max_distance) {
    // blue -> cyan
    double tmp = distance * 5.0 / max_distance;
    answer.r = 0;
    answer.g = tmp*255;
    answer.b = 255;
  } else if (distance < 0.4*max_distance) {
    // cyan -> green
    double tmp = (distance-0.2*max_distance) * 5.0 / max_distance;
    answer.r = 0;
    answer.g = 255;
    answer.b = (1-tmp*tmp)*255;
  } else if (distance < 0.6*max_distance) {
    // green -> yellow
    double tmp = (distance-0.4*max_distance) * 5.0 / max_distance;
    answer.r = sqrt(tmp)*255;
    answer.g = 255;
    answer.b = 0;
  } else if (distance < 0.8*max_distance) {
    // yellow -> red
    double tmp = (distance-0.6*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = (1-tmp*tmp)*255;
    answer.b = 0;
  } else if (distance < max_distance) {
    // red -> white
    double tmp = (distance-0.8*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = tmp*255;
    answer.b = tmp*255;
  } else {
    // white
    answer.r = answer.g = answer.b = 255;
  }  
  return answer;
}

Color GreyBands(double distance, double max_value, int num_bands) {
  Color answer;
  if (distance < 0.001) {
    // red
    answer.r = 255; answer.g = 0; answer.b = 0;
  } else {
    // shades of grey
    answer.r = answer.g = answer.b = int(num_bands*256*distance/double(max_value)) % 256;
  }  
  return answer;
}

// ===================================================================================================