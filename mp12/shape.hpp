#ifndef SHAPE_H_
#define SHAPE_H_

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip> 

using namespace std;

//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initialize Shape's private variable name 
  	Shape(string name) {
		  name_ = name; //this assumes the name of a string is a pointer to its first character

	}
	
  	string getName() {
		  return name_;

	}
	
  	virtual double getArea() const = 0; //so this is completely virtual function - it cannot be called
  	virtual double getVolume() const = 0; //completely virtual function - only a derived class's implementation can be called
	  //what's the point of these above functions?
	
private:
  string name_;


};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
  	Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle") { //initializer list calls constructor in base class
	  width_ = width;
	  length_ = length;


	}
	
  	double getArea() const { //what does const keyword mean?
	  double area = (double) width_ * (double)length_;
	  return area;


	}
	
  	double getVolume() const {
		  return (double) 0;

	}
	
	Rectangle<T> operator + (const Rectangle<T>& rec) { //takes in reference variable

		Rectangle<T> r3(0,0);
		r3.width_ = width_ + rec.width_;
		r3.length_ = length_ + rec.length_;
		return r3;
		


	}
	
	Rectangle<T> operator - (const Rectangle<T>& rec) {
		Rectangle<T> r3(0,0);
		r3.width_ = width_ - rec.width_;
		r3.length_ = length_ - rec.length_;
		if(r3.width_ < 0)
		r3.width_ = 0;
		if(r3.length_ < 0)
		r3.length_ = 0;
		return r3;

	} 
	
	T getWidth() const { 
		return width_;
	
	}
	
	T getLength() const { 
		return length_;
	
	}
private:
	T width_;
    T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
  	Circle(double radius):Shape("Circle") {  //initializer list to initialize name in Shape class
		  radius_ = radius;

	}
	
  	double getArea() const{
		  double area = (double)radius_ * (double)radius_ * M_PI;
		  return area;


	}
	
 	double getVolume() const{
		 return (double) 0;

	}
	
  	Circle operator + (const Circle& cir){
		  Circle c3(0);
		  c3.radius_ = radius_ + cir.radius_;
		  return c3;

	}
	
	Circle operator - (const Circle& cir){
		Circle c3(0);
		c3.radius_ = radius_ - cir.radius_;
		if(c3.radius_ < 0)
		c3.radius_ = 0;
		return c3;

	} 

	double getRadius() const { 
		return radius_;
	
	}
	
private:
	double radius_;

};



//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
  	Sphere(double radius):Shape("Sphere") { //initializer list uses constructor in Shape class to set the name
	  radius_ = radius;

	}

  	double getVolume() const {
		  double volume = (4.0/3.0) * (double)radius_ * (double)radius_ * (double)radius_ * M_PI;
		  return volume;

	}	
	
  	double getArea() const {
		  double area = 4.0 * M_PI * (double)radius_ * (double)radius_;
		  return area;

	}

	Sphere operator + (const Sphere& sph) {
		Sphere s3(0);
		s3.radius_ = radius_ + sph.radius_;
		return s3;

	}

	Sphere operator - (const Sphere& sph) {
		Sphere s3(0);
		s3.radius_ = radius_ - sph.radius_;
		if(s3.radius_ < 0)
		s3.radius_ = 0;
		return s3;

	} 
	
	double getRadius() const {
		return radius_;
		
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
class RectPrism : public Shape{
public:
  	RectPrism(double width, double length, double height):Shape("RectPrism") { //calls initializer list to set name using function in Shape class
	  width_ = width;
	  length_ = length;
	  height_ = height;

	}
	
  	double getVolume() const {
		  double volume = (double)width_ * (double)length_ * (double)height_;
		  return volume;

	}
  	
	double getArea() const {
		double area = 2 * (double)(width_ * height_ + width_ * length_ + height_ * length_);
		return area;

	}
	
	RectPrism operator + (const RectPrism& rectp){
		RectPrism rp3(0,0,0);
		rp3.width_ = width_ + rectp.width_;
		rp3.length_ = length_ + rectp.length_;
		rp3.height_ = height_ + rectp.height_;
		return rp3;


	}
	
	RectPrism operator - (const RectPrism& rectp){
		RectPrism rp3(0,0,0);
		rp3.width_ = width_ - rectp.width_;
		rp3.length_ = length_ - rectp.length_;
		rp3.height_ = height_ - rectp.height_;
		if(rp3.width_ < 0)
		rp3.width_ = 0;
		if(rp3.height_ < 0)
		rp3.height_ = 0;
		if(rp3.length_ < 0)
		rp3.length_ = 0;
		return rp3;


	}	
	
	double getWidth() const { 
		return width_;
	
	}
	
	double getLength() const { 
		return length_;
	
	}
	
	double getHeight() const { 
		return height_;

	}
private:
  	double length_;
  	double width_;
  	double height_;

};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
static list<Shape*> CreateShapes(char* file_name) {

	//@@Insert your code here

	

	double param1,param2, param3;
	string name;
	int numobjects;
	int count = 0;

	
	ifstream ifs("test1.txt",std::ifstream::in);
	ifs>>param1;
	numobjects = param1;
	list<Shape*> shapeslist;


	while(count < numobjects)
	{

		
	ifs>>name;

	

	if(name.compare("Circle") == 0)
	{
		ifs>>param1;
		Circle * cp = new Circle (param1);
		shapeslist.push_back(cp);
	}
	

	
	else if(name.compare("Rectangle") == 0)
	{
		ifs>>param1>>param2;
		Rectangle<double> * rp = new Rectangle<double> (param1,param2);
		shapeslist.push_back(rp);
				
	}
	

	
	else if(name.compare("Sphere") == 0)
	{
		ifs>>param1;
		Sphere * sp = new Sphere (param1);
		shapeslist.push_back(sp);
	}

	else if(name.compare("RectPrism") == 0)
	{
		ifs>>param1>>param2>>param3;
		RectPrism * rpp = new RectPrism (param1, param2, param3);
		shapeslist.push_back(rpp);
	}
	
	count += 1;


	}

	ifs.close();


	return shapeslist;
	//return list<Shape*>(0, NULL);;
}

// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
	for (list<Shape*>::iterator it =shapes.begin(); it != shapes.end(); it++) {
		if ((*it)->getArea() > max_area)
		max_area = (*it)->getArea();
    
}

	//cout<<max_area<<endl;
	
	return max_area;
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
	for (list<Shape*>::iterator it =shapes.begin(); it != shapes.end(); it++) {
		if ((*it)->getVolume() > max_volume)
		max_volume = (*it)->getVolume();
	}
	
	return max_volume;
}
#endif

