
����������˹���ģʽ�Ͳ���ģʽ��

AbstractPizza����Pizza�ĳ�����࣬��װ������Pizza���е�����name��cook��cutted�ȣ�
������Ҫ��д��eat������Ϊ�麯����
Pizza��̳�AbstractPizza�࣬��д��eat�ӿڡ�

Plate����ԭ����ʵ����ȫ��ͬ��

AbstractMakePizza����MakePizza������ĳ�����࣬����ʵ�ַ���ʹ�ù������������Խ���ϡ�
PizzaType������main�����г��ֵ�����pizza���࣬��ö�����װ��ǿ����ɶ��ԡ�
MakePizza��̳�AbstractMakePizza�࣬��д��makePizza�ӿڡ�

PizzaFactory������������Ĺ������ڹ��칤��ʱ�����������ԡ�������vector�ֱ��װ��plate��pizza��
plate��pizza�Ĺ���������ڹ������ڲ���ɡ�

�˼ܹ���϶ȵͣ�����չ��ǿ��ֻҪ�µ�pizza��ʵ����AbstractPizza�Ľӿڣ�
���ʵ����makPizza�ӿڵĿ����������ԣ��Ϳ��Թ����������ֿ��ȵĹ�����
makefile�ļ�Ҳ���п���չ�ԡ�

�ڴ˼ܹ���������д��main����:
int main() {

	/*�����������Զ��󣬲��Դ˹��칤��*/
	MakePizza pizzaMakeStrategy;
	
	PizzaFactory* pizzaFactory = new PizzaFactory(&pizzaMakeStrategy);
	......
	/*��������ʱ�������ڲ���pizza��plate*/
	delete pizzaFactory;
	return 0;
}