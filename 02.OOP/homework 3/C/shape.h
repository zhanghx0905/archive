const double PI = 3.14;

class Shape {
	string id;
public:
	static int count;
	Shape(double *size,const string _type) : size(size) {
		id = _type + " " + to_string(count);
		count++;
	}
	virtual ~Shape() {}

	virtual double get_area() = 0;
	virtual string type() = 0;
	friend ostream &operator <<(ostream &out,Shape* w) {
		return out << w->id << ": " << w->get_area();
	}

protected:
	double* size;
};

class Rectangle :public Shape {
public:
	Rectangle(double *size,const string _type="Rectangle")
		: Shape(size,_type) {}
	virtual ~Rectangle() {
		if (size != nullptr) {
			delete[] size;
			size = nullptr;
		}
	}

	virtual double get_area() override { return size[0]*size[1]; }
	virtual string type() override {
		return "Rectangle";
	}
	int angles() {
		return 4;
	}

};

class Square : public Rectangle {
public:
	Square(double *size, const string _type = "Square")
		: Rectangle(size,_type) {}
	virtual ~Square() {
		if (size != nullptr) {
			delete size;
			size = nullptr;
		}
	}

	virtual double get_area() override { return size[0]*size[0]; }
	virtual string type() override {
		return "Square";
	}
};

class Circle : public Shape {
public:
	Circle(double *size, const string _type = "Circle")
		: Shape(size,_type) {}
	virtual ~Circle() {
		if (size != nullptr) {
			delete size;
			size = nullptr;
		}
	}

	virtual double get_area() override { return PI*size[0]*size[0]; }
	virtual string type() override {
		return "Circle";
	}
};
