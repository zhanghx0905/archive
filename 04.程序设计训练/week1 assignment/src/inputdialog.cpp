#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);

    sBoxInput.resize(10);
    for (int i=0;i<10;++i) {
        sBoxInput[i]=new QSpinBox(this);
        sBoxInput[i]->setMinimum(1);
        if(i%2==0)
            sBoxInput[i]->setMaximum(ui->sBoxRow->value());
        else sBoxInput[i]->setMaximum(ui->sBoxCol->value());

        ui->gLayoutIn->addWidget(sBoxInput[i],i/2,i%2);
        if(i>=2) sBoxInput[i]->setEnabled(false);
      }

}

InputDialog::~InputDialog()
{
    delete ui;
}

QPair<int, int> InputDialog::outputPos()
{
    return QPair<int,int>(ui->sBoxOutX->value(),ui->sBoxOutY->value());
}

int InputDialog::rowNum()
{
    return ui->sBoxRow->value();
}

int InputDialog::colNum()
{
    return ui->sBoxCol->value();
}

bool InputDialog::wash()
{
    return ui->cBoxWash->isChecked();
}

QSet<QPair<int, int> > InputDialog::inputPos()
{
    int inNum=ui->spinBox->value();
    QSet<QPair<int, int> > ans;
    for(int i=0;i<2*inNum;i+=2){
        int x=sBoxInput[i]->value();
        int y=sBoxInput[i+1]->value();
        ans.insert(QPair<int,int>(x,y));
    }
    return ans;
}

void InputDialog::on_spinBox_valueChanged(int arg1)
{
    for(int i=0;i<10;i++){
        sBoxInput[i]->setEnabled(i<2*arg1);
    }
}

void InputDialog::on_sBoxRow_valueChanged(int arg1)
{
    ui->sBoxOutX->setMaximum(arg1);ui->sBoxOutX->setValue(arg1);
    for(int i=0;i<10;i+=2){
        sBoxInput[i]->setMaximum(arg1);
    }

}

void InputDialog::on_sBoxCol_valueChanged(int arg1)
{
    ui->sBoxOutY->setMaximum(arg1);ui->sBoxOutY->setValue(arg1);
    for(int i=1;i<10;i+=2){
        sBoxInput[i]->setMaximum(arg1);
    }
}
void InputDialog::on_cBoxWash_clicked(bool checked)
{
    if(checked){
        QMessageBox::information(this,tr("提示"),tr("若启用清洗液滴，则出入口不能放置在左下或右上!"));
    }
}
