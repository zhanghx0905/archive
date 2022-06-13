程序架构

本程序使用适配器模式，逻辑简单，具有可扩展性。增加了Adapter类，将new_shape类的接口转换给shape类。

Adapter类用模板完成，继承了Shape类的接口和模板类T(Triangle, Rhombus, etc.)的实现。

与此架构相对应，main函数有两处改动：
	Adapter<Triangle>* triangle = new Adapter<Triangle>;
	
	Adapter<Rhombus>* rhombus = new Adapter<Rhombus>;
将模板实例化供已有的Drawing类对象调用。