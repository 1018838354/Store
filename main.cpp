//
//  main.cpp
//  Store
//
//  Created by 曹利坚 on 16/6/11.
//  Copyright © 2016年 曹利坚. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <fstream>
#include <time.h>
//#include <istream>
#define Goodsdata "Goods"//商品数据
#define Buyerdata "Buyer"//买家数据
#define Sellerdata "Seller"//卖家数据
using namespace std;
string supername;//全局卖家名
string Gettime(){//获取当前时间
    string date;
    time_t Time=time(0);
    char s[17];
    sprintf(s, "%d年%d月%d日",(1900+localtime(&Time)->tm_year),(1+localtime(&Time)->tm_mon),(localtime(&Time)->tm_mday));
    date=s;
    return date;
}
class Specifications{//规格
public:
    double height;//高
    double length;//长
    double wide;//宽
};
class Goods{
public:
    string num;//编号
    string name;//名称
    Specifications spe;//规格
    double price;//价格
    int volume;//库存
    int salevolume;//销量
    string style;//类型
    string producingplace;//产地
    string belong;//商家
    string grounding;//上市
    Goods *next;//连接下一个商品
};
class GoodsManage{
private:
    Goods *head;//商品头
    Goods *tail;//商品尾
    Goods *current;//当前操作商品
    int total;//商品数
    vector<Goods> vectGoods;//vector容器
    string passwords;//管理员密码
public:
    GoodsManage():head(NULL),tail(NULL),current(NULL),total(0){Load();}
    ~GoodsManage(){Save();}
    bool Add();//添加商品
    bool AllConfirm(string passwords);//管理认证
    Goods getgoods(){return *current;}//获取当前商品所有信息给买家
    bool Search(string sname,double dname,int style);//精确查找商品
    bool Vagueseek(string sname,int style);//模糊查找
    bool Filter(string sname,double dname,int style);//筛选查找
    bool Editstatus(double change,int style);//修改商品库存或销量或价格情况(注:传参时-1为减少一,1为增加一)
    bool Delete(int num);//删除商品
    bool Load();//读取商品数据
    bool Save();//保存商品数据
    bool Menu();//管理目录
    bool Uppasswords(bool stlye);//密码管理
    bool Editer(int style);//管理数据
    bool Update(string type, int style);//更新数据
    bool Seekmenu();//查找目录
    bool Disvector();//显示vector容器内容 for seek
    bool Display();//显示所有(suoyou)商品
    bool Show();//显示当前定位商品
};
class User{
public:
    string id;//用户ID
    string name;//用户名
    string passwords;//用户密码
    string tel;//电话
    string address;//地址
};
class Buyer:public User{
public:
    Goods *head;//已购买商品头
    Goods *tail;//已购买商品尾
    Goods *current;//已购买商品定位
    int total;//已购买商品数
    double money;//现金
    vector<Goods> ShoppingCart;//购物车
    Buyer *next;//连接下一个买家
};
class BuyerManage{
private:
    Buyer *head;//买家链表头
    Buyer *tail;//买家链表尾
    Buyer *current;//当前买家
    int total;//买家数量
public:
    BuyerManage():head(NULL),tail(NULL),current(NULL),total(0){Load();
    }
    ~BuyerManage(){Save();}
    bool Load();//导入数据
    bool Save();//保存数据
    bool Confirm(string id,string name,string passwords);//买家登陆
    bool Apply();//申请账号
    bool Menu(GoodsManage &Magegoo);//买家管理目录
    bool Buy(GoodsManage &Magegoo);//购买新商品
    Goods Getgood(GoodsManage &Magegoo);//获取商品
    bool Displayshop();//显示该买家所有商品
    bool couttotal(GoodsManage &Magegoo);//结算购物车
    bool Deletebuyer(int num);//
    bool Delete(int num);//删除商品记录
    bool Update(string type,int style);//修改买家信息
    bool Editer(int style);//编辑买家信息
    bool Search(string type,double num,int style);//查找已经购买的商品
    bool Sort(int style);//排序
    bool Display();//显示所有买家信息
    bool Show();//显示当前买家信息
};
class Seller:public User{
public:
    Goods *head;//卖家商品头
    Goods *tail;//卖家商品尾
    Goods *current;//卖家商品定位
    int total;//卖家商品总数
    Seller *next;//卖家连接下一个卖家
    string storename;//店铺名
};
class SellerManage{//以卖家为总类
private:
    Seller *head;//卖家链表头
    Seller *tail;//卖家链表尾
    Seller *current;//当前卖家
    int total;//卖家数量
    //----------包含的子类项
    BuyerManage Magebuy;//实例化买家
    GoodsManage Magegoo;//实例化商品
    
public:
    SellerManage():head(NULL),tail(NULL),current(NULL),total(0){Load();AllMenu();}
    ~SellerManage(){Save();}
    bool Load();//导入数据
    bool Save();//保存数据
    bool Confirm(string id,string name,string passwords);//卖家登陆
    bool Menu();//卖家管理目录
    bool Apply();//申请账号
    bool Add();//添加新商品
    bool Deleteseller(int num);//删除店铺
    bool Delete(int num);//下架商品
    bool Displayshop();//显示该卖家商品
    bool Update(string type,int style);//修改卖家信息
    bool Editer(int style);//编辑卖家信息
    bool Search(string type,double num,int style);//查找正在销售卖的商品
    bool Sort(int style);//排序
    bool Display();//显示所有卖家信息
    bool Show();//显示定位的卖家信息
    bool getname(){supername=current->storename;return true;}//获取卖家名，作全局
    bool DebugMenu();//调试目录
    bool AllMenu();//总管理目录
};
//-------------------------------------------------------------BuyerManage
bool BuyerManage::Confirm(string id,string name,string passwords){//登陆
    int choose;
    if (passwords.length()==0)return false;
    if(id.length()!=0)if(name.length()!=0)choose=1;else choose=3;
    else if (name.length()!=0)choose=2;
    else return false;
    Buyer *p=head;
    switch (choose) {
        case 1://有ID又有用户名
        {
            for (int i=0; i<total; i++) {//用户数量
                if (p->id==id&&p->passwords==passwords)break;
                else p=p->next;
            }if(p!=NULL)break;}
        case 2://只有用户名
            p=head;
            for (int i=0; i<total; i++) {
                if (p->name==name&&p->passwords==passwords)break;
                else p=p->next;
            }
            break;
        case 3://只有ID
            for (int i=0; i<total; i++) {//用户数量
                if (p->id==id&&p->passwords==passwords)break;
                else p=p->next;
            }
            break;
            
        default:cout<<"confirm函数出错\n";
            break;
    }
    if(p!=NULL){current=p;return true;}
    else {current=NULL;return false;}
    return true;
}

bool BuyerManage::Load(){//读取买家数据
    Buyer *p,*p2=NULL;Goods *r,*r2;
    ifstream ifile;
    ifile.open(Buyerdata);
    if (!ifile){ifile.close();
        cout<<"&买家数据导入错误"<<endl;sleep(1);return false;}//if(!ifile.eof())判断是非读结束
    char temp;
    while (ifile.get(temp)){
        if (temp == ':') break;
    }
    ifile >> total;
    if (total<=0){ifile.close();return false;}
    for (int i = 0; i < total; i++)
    {
        if (i==0) {
            p=head=new Buyer;
        }else{p2->next=p=new Buyer;}
        ifile>>p->id>>p->name>>p->passwords>>p->tel>>p->address>>p->total>>p->money;
        if (p->total>0) {
            p->head=new Goods;
            ifile>>p->head->num>>p->head->name>>p->head->spe.length>>p->head->spe.wide>>p->head->spe.height>>p->head->price>>p->head->volume>>p->head->salevolume>>p->head->style>>p->head->producingplace>>p->head->belong>>p->head->grounding;
            p->head->next=NULL;p->tail=p->head;r2=p->head;//该会员首本书导入完成
            
            for (int i = 1; i < p->total; i++)
            {
                r2->next=r=new Goods;
                ifile>>r->num>>r->name>>r->spe.length>>r->spe.wide>>r->spe.height>>r->price>>r->volume>>r->salevolume>>r->style>>r->producingplace>>r->belong>>r->grounding;
                p->tail=r2=r;
            }r2->next=NULL;}
        p->next=NULL;p->current=NULL;//该会员倒入完成
        tail=p2=p;//该会员倒入完成
    }
    ifile.close();cout<<"&买家数据导入成功"<<endl;sleep(1);
    return true;
}
bool BuyerManage::Save(){//保存买家数据
    Buyer *p=head;Goods *r;
    fstream ofile;
    ofile.open(Buyerdata,ios::out);
    ofile <<"此文件为买家数据库DataVolume:"<<total<<endl;//有多少个会员
    for (int i = 0; i < total; i++)
    {
        ofile << p->id << '\t' << p->name<< '\t'<< p->passwords<< '\t'<<p->tel<< '\t'<<p->address<< '\t'<<p->total<< '\t'<<p->money<<endl;//会员买了多少商品
        r=p->head;
        for (int i = 0; i < p->total; i++)
        {
        ofile << r->num << '\t' << r->name<< '\t'<< r->spe.length<< '\t'<< r->spe.wide<< '\t'<< r->spe.height<< '\t' << r->price<< '\t' << r->volume<< '\t' << r->salevolume<< '\t' << r->style<< '\t' << r->producingplace<< '\t' << r->belong<< '\t' << r->grounding<< endl;r=r->next;
        }
        p=p->next;
    }
    ofile.close();
    return true;
}
bool BuyerManage::Apply(){//添加会员
    char type;
    do{
        Buyer *mnew=new Buyer;
        cout<<"用户ID号:";cin>>mnew->id;
        cout<<"用户名:";cin>>mnew->name;
        cout<<"密码:";cin>>mnew->passwords;
        cout<<"电话:";cin>>mnew->tel;
        cout<<"地址:";cin>>mnew->address;
        mnew->total=0;
        mnew->next=NULL;mnew->head=NULL;mnew->current=NULL;mnew->tail=NULL;
        if (total==0) {
            head=mnew;
        }else{
            tail->next=mnew;}
        tail=mnew;total++;//会员人数加1
        cout<<"是否继续申请? Y/N";
        cin>>type;
    }while(type!='N'&&type!='n');
    return true;
}

bool BuyerManage::Editer(int style){
    int type2;string temp1;Buyer *p=head;
    if (style>0) {
        if(style>total){cout<<"错误范围"<<endl;return false;}
        for (int i=0; i<style-1; i++) {
            p=p->next;
        }current=p;style=0;
    }else if(style<0){cout<<"错误范围"<<endl;return false;}
    if (style==0) {
        while(1){
            Show();
        cout<<"1.修改您的ID"<<endl;
        cout<<"2.修改您的登陆密码"<<endl;
        cout<<"3.修改您的名称"<<endl;
        cout<<"4.修改您的电话"<<endl;
        cout<<"5.修改您的地址"<<endl;
        cout<<"6.注销该买家"<<endl;
        cout<<"0.返回"<<endl;
        cout<<"请输入:"<<endl;cin>>type2;
        if (type2>=1&&type2<6) {
            cout<<"请输入你要修改后的数据:"<<endl;cin>>temp1;
            Update(temp1, type2);}else if(type2==6){
                if(Deletebuyer(0)==true){cout<<"删除成功！"<<endl;return true;}else
                    cout<<"删除失败！"<<endl;sleep(1);return false;}
            else{cout<<"正在返回"<<endl;sleep(1);return true;}}
    }
    return true;
}
bool BuyerManage::Menu(GoodsManage &Magegoo){
    int type;string temp1;double type1;
    while (1) {
        cout<<"尊敬:"<<current->name<<"先生/女士"<<" 你当前账户余额:"<<current->money<<endl;
        cout<<"当前已购买商品数量:"<<current->total<<endl;
        cout<<"1.查看所有已购买商品\n";
        cout<<"2.购买新商品\n";
        cout<<"3.结算购物车\n";
        cout<<"4.删除已购买商品记录\n";
        cout<<"5.排序已购买商品\n";
        cout<<"6.修改个人信息\n";
        cout<<"7.充值账户\n";
        cout<<"0.返回\n";
        cin>>type;
        switch (type) {
            case 1:
                while(1){
                    if (total==0) {
                        cout<<"无数据"<<endl;sleep(1);break;}
                    Displayshop();
                    cout<<"输入你要删除的商品(输入0返回)\n";
                    cout<<"请输入:"<<endl;
                    cin>>type;if(type==0){cout<<"正在返回"<<endl;break;}
                    else{if(Delete(type)){cout<<"删除成功"<<endl;}else cout<<"删除失败"<<endl;}sleep(1);}
                
                break;
            case 2:
                if(Buy(Magegoo)==false){cout<<"查询失败"<<endl;sleep(1);};
                break;
            case 3:
                couttotal(Magegoo);break;
            case 4:{
                cout<<"请输入你要删除的已购买商品名称:";cin>>temp1;
                if(Search(temp1, 1,0)==true){
                    if(Delete(0)==true)cout<<"删除成功!"<<endl;else cout<<"删除失败!"<<endl;}
                sleep(1);}
                break;
            case 5:{
                cout<<"请选择按什么排序"<<endl;
                cout<<"1.商品编号"<<endl;
                cout<<"2.商品名称"<<endl;
                cout<<"3.价格－缺省"<<endl;
                cout<<"4.价格"<<endl;
                cout<<"5.库存"<<endl;
                cout<<"6.销量"<<endl;
                cout<<"7.类型"<<endl;
                cout<<"8.产地"<<endl;
                cout<<"9.商家"<<endl;
                cout<<"10.上架时间"<<endl;
                cout<<"0.返回"<<endl;
                cout<<"请输入:";cin>>type;
                if (type<=10&&type>0) {
                    Sort(type);
                }else{cout<<"正在返回"<<endl;sleep(1);}
                break;
            case 6:
                Editer(0);break;
            case 7:
                do{
                    cout<<"请输入你要充值金额(输入0直接返回):";cin>>type1;
                    if (type1>0) {current->money+=type1;
                        cout<<"成功充值！"<<endl;sleep(1);break;
                    }else{cout<<"充值错误！"<<endl;sleep(1);}
                    }while((int)type1!=0);cout<<"正在返回！"<<endl;sleep(1);
                break;
            default:
                cout<<"正在返回"<<endl;sleep(1);
                return true;
            }
        }
    }return true;
}
bool BuyerManage::couttotal(GoodsManage &Magegoo){
    double thistotal,totalmoney;int type,type3,type4;char ch;
    while(1){
        totalmoney=0;
        if(current->ShoppingCart.size()==0){cout<<"你的购物车为空！"<<endl;sleep(1);return false;}
    for (int i=0; current->ShoppingCart.size(); i++) {
        thistotal=(current->ShoppingCart[i].price*current->ShoppingCart[i].volume);
    cout<<"序号:"<<i+1<<" 商品编号:"<<current->ShoppingCart[i].num<<" 名称:"<<current->ShoppingCart[i].name<<" 规格:"<<current->ShoppingCart[i].spe.length<<"*"<<current->ShoppingCart[i].spe.wide<<"*"<<current->ShoppingCart[i].spe.height;
        cout<<" 价格:"<<current->ShoppingCart[i].price<<" 购买数量:"<<current->ShoppingCart[i].volume<<" 合计:"<<thistotal<<" 类型:"<<current->ShoppingCart[i].style<<" 产地:"<<current->ShoppingCart[i].producingplace<<" 商家:"<<current->ShoppingCart[i].belong<<" 上架日期:"<<current->ShoppingCart[i].grounding<<endl;totalmoney+=thistotal;
        current->ShoppingCart[i].salevolume=thistotal;
    }
        cout<<"当前所有商品合计:"<<totalmoney<<" 元 ";
        if(totalmoney>current->money){cout<<"你当前余额不足！请先充值或删除一些商品\n";sleep(1);}
        else cout<<endl;
        cout<<"请选择你要操作的商品序号:"<<endl;
        cout<<"0.购买或返回"<<endl;
        cout<<"请输入:";cin>>type;
        if (type>=0&&type<=current->ShoppingCart.size()) {
            if (type==0) {
                cout<<"确认购买(Y)返回(N)请输入:";cin>>ch;if(ch!='Y'||ch!='y')return false;
                for (int i=0; current->ShoppingCart.size(); i++) {
                    Goods *datanew=new Goods;
                    *datanew=current->ShoppingCart[i];
                    if(datanew->salevolume<current->money){
                    datanew->next=NULL;
                    if (current->total==0) {
                        current->head=datanew;
                    }else{
                        current->tail->next=datanew;}
                        current->tail=datanew;current->total++;
                        current->money-=datanew->salevolume;
                    }else{cout<<"商品 "<<datanew->name<<"购买失败"<<endl;sleep(1);}
                }
                current->ShoppingCart.clear();
            }else {
                while(1){cout<<" 商品编号:"<<current->ShoppingCart[type].num<<" 名称:"<<current->ShoppingCart[type].name<<" 规格:"<<current->ShoppingCart[type].spe.length<<"*"<<current->ShoppingCart[type].spe.wide<<"*"<<current->ShoppingCart[type].spe.height;
                    cout<<" 价格:"<<current->ShoppingCart[type].price<<" 购买数量:"<<current->ShoppingCart[type].volume<<" 合计:"<<current->ShoppingCart[type].salevolume<<" 类型:"<<current->ShoppingCart[type].style<<" 产地:"<<current->ShoppingCart[type].producingplace<<" 商家:"<<current->ShoppingCart[type].belong<<" 上架日期:"<<current->ShoppingCart[type].grounding<<endl;
                cout<<"1.修改数量"<<endl;
                cout<<"2.删除此商品"<<endl;
                cout<<"0.完成修改"<<endl;
                cout<<"请输入:"<<endl;cin>>type3;
                if (type3==0) {
                    break;
                }else if (type3==1){cout<<"请输入你要购买的数量:";cin>>type4;while(type4<0){cout<<"请重新输入:";cin>>type4;}
                    current->ShoppingCart[type].volume=type4;cout<<"修改成功"<<endl;sleep(1);
                }else if (type3==2){vector<Goods>::iterator it=current->ShoppingCart.begin();current->ShoppingCart.erase(it+type-1);cout<<"删除成功"<<endl;sleep(1);break;}
                }
            }
        }
        
    }
    return true;
}
bool BuyerManage::Buy(GoodsManage &Magegoo){
    int type2,type3;char type;
    if(Magegoo.Seekmenu()==false)return false;
    cout<<"1.直接购买"<<endl;
    cout<<"2.添加购物车"<<endl;
    cout<<"0.返回"<<endl;
    cout<<"请输入:";cin>>type2;
    switch (type2) {
        case 1:{
                cout<<"请输入你要购买的数量"<<endl;
                cin>>type3;
                Goods *datanew=new Goods;*datanew=Magegoo.getgoods();
            if (type3<=0||datanew->volume<type3){cout<<"数量不存在或数量不足"<<endl;sleep(1);break;}
                datanew->volume=type3;datanew->salevolume=type3*datanew->price;
                if(current->money<datanew->salevolume){cout<<"余额不足！\n";sleep(1);return false;}
                if (Magegoo.Editstatus(-type3, 5)) {
                    Magegoo.Editstatus(type3, 6);
                    current->money-=datanew->salevolume;
                    Magegoo.Editstatus(-type3, 5);Magegoo.Editstatus(type3, 6);
                    datanew->next=NULL;
                if (current->total==0) {
                    current->head=datanew;
                }else{
                    current->tail->next=datanew;}
                    current->tail=datanew;current->total++;
                }
            cout<<"购买成功！"<<endl;sleep(1);
            break;}
        case 2:{
            Goods go=Magegoo.getgoods();
            go.volume=0;
            current->ShoppingCart.push_back(go);
            cout<<"添加成功"<<endl;sleep(1);
            cout<<"是否现在结算购物车？Y/N"<<endl;cin>>type;
            if (type!='Y'&&type!='y') {
                couttotal(Magegoo);
            }
            break;
        }
        default:cout<<"正在返回"<<endl;sleep(1);
            return true;
    }
    return true;
}
bool BuyerManage::Search(string type,double num,int style){
    Goods *p=current->head;
    switch (style) {
        case 0:
            for (int i=0; i<num-1; i++) {
                p=p->next;
            }current->current=p;
            break;
        case 1:
            for (int i=0; i<current->total; i++) {
                if (p->num==type)break;
                else p=p->next;
            }
            break;
        case 2:
            for (int i=0; i<current->total; i++) {
                if (p->name==type)break;
                else p=p->next;
            }
            break;
        case 4:
            for (int i=0; i<current->total; i++) {
                if (p->price==num)break;
                else p=p->next;
            }
            break;
        case 5:
            for (int i=0; i<current->total; i++) {
                if (p->volume==num)break;
                else p=p->next;
            }
            break;
        case 6:
            for (int i=0; i<current->total; i++) {
                if (p->salevolume==num)break;
                else p=p->next;
            }
            break;
        case 7:
            for (int i=0; i<current->total; i++) {
                if (p->style==type)break;
                else p=p->next;
            }
        case 8:
            for (int i=0; i<current->total; i++) {
                if (p->producingplace==type)break;
                else p=p->next;
            }
        case 9:
            for (int i=0; i<current->total; i++) {
                if (p->belong==type)break;
                else p=p->next;
            }
        case 10:
            for (int i=0; i<current->total; i++) {
                if (p->grounding==type)break;
                else p=p->next;
            }
            break;
        default:cout<<"发生错误code:1.search \n";
            break;
    }
    if(p!=NULL){current->current=p;cout<<"找到此数据"<<endl;sleep(1);return true;}
    else {current->current=NULL;cout<<"未找到此数据"<<endl;sleep(1);return false;}
}
bool BuyerManage::Update(string type, int style){
    Buyer *p=current;
    switch (style) {
        case 1:p->id=type;
            break;
        case 2:p->passwords=type;
            break;
        case 3:p->name=type;
            break;
        case 4:p->tel=type;
            break;
        case 5:p->address=type;
            break;
        default:cout<<"发生错误code:1.update \n";sleep(1);
            break;
    }
    return true;
}
bool BuyerManage::Deletebuyer(int num){
    Buyer *p=head,*p2;
    switch (num) {
        case 0:
        {
            if (p==current) {
                total--;
                if(total==0){delete p;head=tail=NULL;}
                else {p2=p;head=p->next;delete p2;}
                current=NULL;return true;
            }else
                for (int i=0; i<total; i++) {
                    if (p->next==current)break;
                    else p=p->next;
                }
        }
            break;
        default:
        {
            if (num==1) {//判断是否为首个
                total--;
                if(total==0){delete head;tail=head=NULL;}//判断是否只有一个
                else {p2=p;head=p->next;delete p2;}
                current=NULL;return true;
            }else
                for (int i=0; i<num-2; i++) {
                    p=p->next;
                }
        }
            break;
    }
    p2=p->next;total--;
    if (p->next->next!=NULL) {//判断是否为尾巴
        p->next=p->next->next;
    }else {p->next=NULL;tail=p;}
    delete p2;current=NULL;
    return true;
}
bool BuyerManage::Delete(int num){
    Goods *p=current->head,*p2;
    switch (num) {
        case 0:
        {
            if (p==current->current) {
                current->total--;
                if(current->total==0){delete p;current->head=current->tail=NULL;}
                else {p2=p;current->head=p->next;delete p2;}
                current->current=NULL;return true;
            }else
                for (int i=0; i<current->total; i++) {
                    if (p->next==current->current)break;
                    else p=p->next;
                }
        }
            break;
        default:
        {
            if (num==1) {//判断是否为首个
                current->total--;
                if(current->total==0){delete current->head;current->tail=current->head=NULL;}//判断是否只有一个
                else {p2=p;current->head=p->next;delete p2;}
                current->current=NULL;return true;
            }else
                for (int i=0; i<num-2; i++) {
                    p=p->next;
                }
        }
            break;
    }
    p2=p->next;current->total--;
    if (p->next->next!=NULL) {//判断是否为尾巴
        p->next=p->next->next;
    }else {p->next=NULL;current->tail=p;}
    delete p2;current->current=NULL;
    return true;
}
bool BuyerManage::Sort(int style){
    if (current->total<2) {
        cout<<"数据太少！无需排序。"<<endl;
        sleep(1);
        return false;
    }
    Goods *p,*p1,*p2,*op,*tp=NULL;
    string compare;double compare2;
    switch (style) {
        case 1:
        {
            compare=current->head->num;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->num) {
                    compare=p->next->num;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->num;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->num) {
                        compare=p->next->num;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 2:
        {
            compare=current->head->name;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->name) {
                    compare=p->next->name;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->name;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->name) {
                        compare=p->next->name;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 4:
        {
            compare2=current->head->price;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare2>p->next->price) {
                    compare2=p->next->price;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare2=op->next->price;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare2>p->next->price) {
                        compare2=p->next->price;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 5:
        {
            compare2=current->head->volume;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare2>p->next->volume) {
                    compare2=p->next->volume;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare2=op->next->volume;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare2>p->next->volume) {
                        compare2=p->next->volume;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
            
        case 6:
        {
            compare2=current->head->salevolume;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare2>p->next->salevolume) {
                    compare2=p->next->salevolume;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare2=op->next->salevolume;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare2>p->next->salevolume) {
                        compare2=p->next->price;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 7:
        {
            compare=current->head->style;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->style) {
                    compare=p->next->style;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->style;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->style) {
                        compare=p->next->style;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 8:
        {
            compare=current->head->producingplace;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->producingplace) {
                    compare=p->next->producingplace;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->producingplace;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->producingplace) {
                        compare=p->next->producingplace;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 9:
        {
            compare=current->head->belong;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->belong) {
                    compare=p->next->belong;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->belong;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->belong) {
                        compare=p->next->belong;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 10:
        {
            compare=current->head->grounding;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->grounding) {
                    compare=p->next->grounding;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->grounding;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->grounding) {
                        compare=p->next->grounding;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
            
        default:
            cout<<"发生错误code:1.sort"<<endl;sleep(1);
            return false;
    }
    current=NULL;
    cout<<"排序完成！"<<endl;sleep(1);
    return true;
}
bool BuyerManage::Display(){
    cout<<"探针:"<<" 总:"<<total<<" 头针:"<<head<<" 尾针:"<<tail<<endl;
    Buyer *p=head;
    for(int i=0;i<total;i++){
        cout<<"序号:"<<i+1<<"ID:"<<p->id<<" 名称:"<<p->name<<" 密码:"<<p->passwords<<" 电话:"<<p->tel<<" 地址:"<<p->address<<" 购买商品数:"<<p->total<<endl;
        p=p->next;
    }
    return true;}
bool BuyerManage::Displayshop(){
    cout<<"探针:"<<" 总:"<<current->total<<" 头针:"<<current->head<<" 尾针:"<<current->tail<<endl;
    Goods *p=current->head;
    for(int i=0;i<current->total;i++){
        cout<<"序号:"<<i+1<<" 商品编号:"<<p->num<<" 名称:"<<p->name<<" 规格:"<<p->spe.length<<"*"<<p->spe.wide<<"*"<<p->spe.height;
        cout<<" 价格:"<<p->price<<" 购买数量:"<<p->volume<<" 合计:"<<p->salevolume<<" 类型:"<<p->style<<" 产地:"<<p->producingplace<<" 商家:"<<p->belong<<" 上架日期:"<<p->grounding<<endl;
        p=p->next;
    }
    return true;}
bool BuyerManage::Show(){
    Buyer *p=current;
    cout<<"ID:"<<p->id<<" 名称:"<<p->name<<" 密码:"<<p->passwords<<" 电话:"<<p->tel<<" 地址:"<<p->address<<" 购买商品数:"<<p->total<<endl;
    return true;}
//-------------------------------------------------------------SellerManage
bool SellerManage::Confirm(string id,string name,string passwords){//登陆
    int choose;
    if (passwords.length()==0)return false;
    if(id.length()!=0)if(name.length()!=0)choose=1;else choose=3;
    else if (name.length()!=0)choose=2;
    else return false;
    Seller *p=head;
    switch (choose) {
        case 1://有ID又有用户名
        {
            for (int i=0; i<total; i++) {//用户数量
                if (p->id==id&&p->passwords==passwords)break;
                else p=p->next;
            }if(p!=NULL)break;
        }
        case 2://只有用户名
            p=head;
            for (int i=0; i<total; i++) {
                if (p->name==name&&p->passwords==passwords)break;
                else p=p->next;
            }
            break;
        case 3://只有ID
            for (int i=0; i<total; i++) {//用户数量
                if (p->id==id&&p->passwords==passwords)break;
                else p=p->next;
            }
            break;
            
        default:cout<<"confirm函数出错\n";
            break;
    }
    if(p!=NULL){current=p;getname();return true;}
    else {current=NULL;return false;}
    return true;
}

bool SellerManage::Load(){//读取卖家数据
    Seller *p,*p2=NULL;Goods *r,*r2;
    ifstream ifile;
    ifile.open(Sellerdata);
    if (!ifile){ifile.close();
        cout<<"&卖家数据导入错误"<<endl;sleep(1);return false;}//if(!ifile.eof())判断是非读结束
    char temp;
    while (ifile.get(temp)){
        if (temp == ':'){ break;}
    }
    ifile >> total;
    if (total<=0){ifile.close();return false;}
    for (int i = 0; i < total; i++)
    {
        if (i==0) {
            p=head=new Seller;
        }else{
            p2->next=p=new Seller;}
        ifile>>p->id>>p->name>>p->passwords>>p->tel>>p->address>>p->total>>p->storename;
        if (p->total>0) {
            p->head=new Goods;
            ifile>>p->head->num>>p->head->name>>p->head->spe.length>>p->head->spe.wide>>p->head->spe.height>>p->head->price>>p->head->volume>>p->head->salevolume>>p->head->style>>p->head->producingplace>>p->head->belong>>p->head->grounding;
            p->head->next=NULL;p->tail=p->head;r2=p->head;//该会员首本书导入完成
            for (int i = 1; i < p->total; i++)
            {
                r2->next=r=new Goods;
                ifile>>r->num>>r->name>>r->spe.length>>r->spe.wide>>r->spe.height>>r->price>>r->volume>>r->salevolume>>r->style>>r->producingplace>>r->belong>>r->grounding;
                p->tail=r2=r;
            }r2->next=NULL;}
        p->next=NULL;p->current=NULL;//该会员倒入完成
        tail=p2=p;//该会员倒入完成
    }
    ifile.close();cout<<"&卖家数据导入成功"<<endl;sleep(1);
    return true;
}
bool SellerManage::Save(){//保存卖家数据
    Seller *p=head;Goods *r;
    fstream ofile;
    ofile.open(Sellerdata,ios::out);
    ofile <<"此文件为卖家数据库DataVolume:"<<total<<endl;//有多少个会员
    for (int i = 0; i < total; i++)
    {
        ofile << p->id << '\t' << p->name<< '\t'<< p->passwords<< '\t'<<p->tel<< '\t'<<p->address<< '\t'<<p->total<<'\t'<<p->storename<<endl;//会员买了多少商品
        r=p->head;
        for (int i = 0; i < p->total; i++)
        {
            ofile << r->num << '\t' << r->name<< '\t'<< r->spe.length<< '\t'<< r->spe.wide<< '\t'<< r->spe.height<< '\t' << r->price<< '\t' << r->volume<< '\t' << r->salevolume<< '\t' << r->style<< '\t' << r->producingplace<< '\t' << r->belong<< '\t' << r->grounding<< endl;
            r=r->next;
        }
        p=p->next;
    }
    ofile.close();
    return true;
}
bool SellerManage::Editer(int style){
    int type2;string temp1;Seller *p=head;
    if (style>0) {
        if(style>total){cout<<"错误范围"<<endl;return false;}
        for (int i=0; i<style-1; i++) {
            p=p->next;
        }current=p;style=0;
    }else if(style<0){cout<<"错误范围"<<endl;return false;}
    if (style==0) {
        while(1){
            Show();
            cout<<"1.修改您的ID"<<endl;
            cout<<"2.修改您的登陆密码"<<endl;
            cout<<"3.修改您的名称"<<endl;
            cout<<"4.修改您的电话"<<endl;
            cout<<"5.修改您的地址"<<endl;
            cout<<"6.注销该卖家"<<endl;
            cout<<"0.返回"<<endl;
            cout<<"请输入:"<<endl;cin>>type2;
            if (type2>=1&&type2<6) {
                cout<<"请输入你要修改后的数据:"<<endl;cin>>temp1;
                Update(temp1, type2);}else if(type2==6){
                    if(Deleteseller(0)==true){cout<<"删除成功！"<<endl;return true;}else
                        cout<<"删除失败！"<<endl;sleep(1);return false;}
            else{cout<<"正在返回"<<endl;sleep(1);return true;}}
    }
    return true;
}
bool SellerManage::Apply(){//添加会员
    char type;
    do{
        Seller *mnew=new Seller;
        cout<<"用户ID号:";cin>>mnew->id;
        cout<<"用户名:";cin>>mnew->name;
        cout<<"店铺名(设置后无法再修改):";cin>>mnew->storename;
        cout<<"密码:";cin>>mnew->passwords;
        cout<<"电话:";cin>>mnew->tel;
        cout<<"地址:";cin>>mnew->address;
        mnew->total=0;
        mnew->next=NULL;mnew->head=NULL;mnew->current=NULL;mnew->tail=NULL;
        if (total==0) {
            head=mnew;
        }else{
            tail->next=mnew;}
        tail=mnew;total++;//会员人数加1
        cout<<"是否继续申请? Y/N";
        cin>>type;
    }while(type!='N'&&type!='n');
    return true;
}

bool SellerManage::Menu(){
    int type;string temp1;getname();
    while (1) {
        cout<<"尊敬:"<<current->name<<"先生/女士"<<endl;
        cout<<"当前本店在售商品数量:"<<current->total<<endl;
        cout<<"1.查看本店所有商品\n";
        cout<<"2.添加新商品\n";
        cout<<"3.删除在售商品\n";
        cout<<"4.排序本店商品\n";
        cout<<"5.修改个人信息\n";
        cout<<"0.返回\n";
        cin>>type;
        switch (type) {
            case 1:
                while(1){
                if (total==0) {
                    cout<<"无数据"<<endl;sleep(1);break;}
                Displayshop();
                cout<<"输入你要删除的商品(输入0返回)\n";
                cout<<"请输入:"<<endl;
                    cin>>type;if(type==0){cout<<"正在返回"<<endl;break;}
                    else{if(Delete(type)){cout<<"删除成功"<<endl;}else cout<<"删除失败"<<endl;}sleep(1);}
                break;
            case 2:Add();
                break;
            case 3:{
                cout<<"请输入你要删除的本店商品名称:";cin>>temp1;
                if(Search(temp1, 1,0)==true){
                    if(Delete(0)==true)cout<<"删除成功!"<<endl;else cout<<"删除失败!"<<endl;}
                sleep(1);}
                break;
            case 4:{
                cout<<"请选择按什么排序"<<endl;
                cout<<"1.商品编号"<<endl;
                cout<<"2.商品名称"<<endl;
                cout<<"3.价格－缺省"<<endl;
                cout<<"4.价格"<<endl;
                cout<<"5.库存"<<endl;
                cout<<"6.销量"<<endl;
                cout<<"7.类型"<<endl;
                cout<<"8.产地"<<endl;
                cout<<"9.商家－－"<<endl;
                cout<<"10.上架时间"<<endl;
                cout<<"0.返回"<<endl;
                cout<<"请输入:";cin>>type;
                if (type<=10&&type>0) {
                    Sort(type);
                }else{cout<<"正在返回"<<endl;sleep(1);}
                break;
            case 5:
                Editer(0);break;
            default:
                cout<<"正在返回"<<endl;sleep(1);
                return true;
            }
        }
    }return true;
}
bool SellerManage::Add(){
    char type;
    do{
        Magegoo.Add();
        Goods *datanew=new Goods;
        *datanew=Magegoo.getgoods();
        datanew->next=NULL;
        if (current->total==0) {
            current->head=datanew;
        }else{
            current->tail->next=datanew;}
        current->tail=datanew;current->total++;
        cout<<"是否继续添加? Y/N";
        cin>>type;
    }while(type!='N'&&type!='n');
    return true;
}
bool SellerManage::Displayshop(){
    cout<<"探针:"<<" 总:"<<current->total<<" 头针:"<<current->head<<" 尾针:"<<current->tail<<endl;
    Goods *p=current->head;
    for(int i=0;i<current->total;i++){
        cout<<"序号:"<<i+1<<" 商品编号:"<<p->num<<" 名称:"<<p->name<<" 规格:"<<p->spe.length<<"*"<<p->spe.wide<<"*"<<p->spe.height;
        cout<<" 价格:"<<p->price<<" 库存:"<<p->volume<<" 销量:"<<p->salevolume<<" 类型:"<<p->style<<" 产地:"<<p->producingplace<<" 商家:"<<p->belong<<" 上架日期:"<<p->grounding<<endl;
        p=p->next;
    }
    return true;}
bool SellerManage::Search(string type,double num,int style){
    Goods *p=current->head;
    switch (style) {
        case 0:
            for (int i=0; i<num-1; i++) {
                p=p->next;
            }current->current=p;
            break;
        case 1:
            for (int i=0; i<current->total; i++) {
                if (p->num==type)break;
                else p=p->next;
            }
            break;
        case 2:
            for (int i=0; i<current->total; i++) {
                if (p->name==type)break;
                else p=p->next;
            }
            break;
        case 4:
            for (int i=0; i<current->total; i++) {
                if (p->price==num)break;
                else p=p->next;
            }
            break;
        case 5:
            for (int i=0; i<current->total; i++) {
                if (p->volume==num)break;
                else p=p->next;
            }
            break;
        case 6:
            for (int i=0; i<current->total; i++) {
                if (p->salevolume==num)break;
                else p=p->next;
            }
            break;
        case 7:
            for (int i=0; i<current->total; i++) {
                if (p->style==type)break;
                else p=p->next;
            }
        case 8:
            for (int i=0; i<current->total; i++) {
                if (p->producingplace==type)break;
                else p=p->next;
            }
        case 9:
            for (int i=0; i<current->total; i++) {
                if (p->belong==type)break;
                else p=p->next;
            }
        case 10:
            for (int i=0; i<current->total; i++) {
                if (p->grounding==type)break;
                else p=p->next;
            }
            break;
        default:cout<<"发生错误code:1.search \n";
            break;
    }
    if(p!=NULL){current->current=p;cout<<"找到此数据"<<endl;sleep(1);return true;}
    else {current->current=NULL;cout<<"未找到此数据"<<endl;sleep(1);return false;}
}
bool SellerManage::Update(string type, int style){
    Seller *p=current;
    switch (style) {
        case 1:p->id=type;
            break;
        case 2:p->passwords=type;
            break;
        case 3:p->name=type;
            break;
        case 4:p->tel=type;
            break;
        case 5:p->address=type;
            break;
        default:cout<<"发生错误code:1.update \n";sleep(1);
            break;
    }
    return true;
}
bool SellerManage::Deleteseller(int num){
    Seller *p=head,*p2;
    switch (num) {
        case 0:
        {
            if (p==current) {
                total--;
                if(total==0){delete p;head=tail=NULL;}
                else {p2=p;head=p->next;delete p2;}
                current=NULL;return true;
            }else
                for (int i=0; i<total; i++) {
                    if (p->next==current)break;
                    else p=p->next;
                }
        }
            break;
        default:
        {
            if (num==1) {//判断是否为首个
                total--;
                if(total==0){delete head;tail=head=NULL;}//判断是否只有一个
                else {p2=p;head=p->next;delete p2;}
                current=NULL;return true;
            }else
                for (int i=0; i<num-2; i++) {
                    p=p->next;
                }
        }
            break;
    }
    p2=p->next;total--;
    if (p->next->next!=NULL) {//判断是否为尾巴
        p->next=p->next->next;
    }else {p->next=NULL;tail=p;}
    delete p2;current=NULL;
    return true;
}
bool SellerManage::Delete(int num){
    Goods *p=current->head,*p2;
    switch (num) {
        case 0:
        {if(Magegoo.Search(current->current->num, 0, 1)){Magegoo.Delete(0);}else{cout<<"商品已不在售！"<<endl;sleep(1);}
            if (p==current->current) {
                current->total--;
                if(current->total==0){delete p;current->head=current->tail=NULL;}
                else {p2=p;current->head=p->next;delete p2;}
                current->current=NULL;return true;
            }else
                for (int i=0; i<current->total; i++) {
                    if (p->next==current->current)break;
                    else p=p->next;
                }
        }
            break;
        default:
        {
            for (int i=0; i<num-1; i++) {
                p=p->next;
            }
            if(Magegoo.Search(p->num, 0, 1)){Magegoo.Delete(0);}else{cout<<"商品已不在售！"<<endl;sleep(1);}
            p=current->head;
            if (num==1) {//判断是否为首个
                current->total--;
                if(current->total==0){delete current->head;current->tail=current->head=NULL;}//判断是否只有一个
                else {p2=p;current->head=p->next;delete p2;}
                current->current=NULL;return true;
            }else
                for (int i=0; i<num-2; i++) {
                    p=p->next;
                }
        }
            break;
    }
    p2=p->next;current->total--;
    if (p->next->next!=NULL) {//判断是否为尾巴
        p->next=p->next->next;
    }else {p->next=NULL;current->tail=p;}
    delete p2;current->current=NULL;
    return true;
}
bool SellerManage::Sort(int style){
    if (current->total<2) {
        cout<<"数据太少！无需排序。"<<endl;
        sleep(1);
        return false;
    }
    Goods *p,*p1,*p2,*op,*tp=NULL;
    string compare;double compare2;
    switch (style) {
        case 1:
        {
            compare=current->head->num;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->num) {
                    compare=p->next->num;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->num;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->num) {
                        compare=p->next->num;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 2:
        {
            compare=current->head->name;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->name) {
                    compare=p->next->name;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->name;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->name) {
                        compare=p->next->name;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
            case 4:
        {
            compare2=current->head->price;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare2>p->next->price) {
                    compare2=p->next->price;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare2=op->next->price;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare2>p->next->price) {
                        compare2=p->next->price;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 5:
        {
            compare2=current->head->volume;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare2>p->next->volume) {
                    compare2=p->next->volume;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare2=op->next->volume;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare2>p->next->volume) {
                        compare2=p->next->volume;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;

        case 6:
        {
            compare2=current->head->salevolume;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare2>p->next->salevolume) {
                    compare2=p->next->salevolume;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare2=op->next->salevolume;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare2>p->next->salevolume) {
                        compare2=p->next->price;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 7:
        {
            compare=current->head->style;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->style) {
                    compare=p->next->style;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->style;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->style) {
                        compare=p->next->style;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 8:
        {
            compare=current->head->producingplace;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->producingplace) {
                    compare=p->next->producingplace;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->producingplace;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->producingplace) {
                        compare=p->next->producingplace;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 9:
        {
            compare=current->head->belong;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->belong) {
                    compare=p->next->belong;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->belong;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->belong) {
                        compare=p->next->belong;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;
        case 10:
        {
            compare=current->head->grounding;
            p=current->head;
            for (int j=0; j<current->total-1; j++) {
                if (compare>p->next->grounding) {
                    compare=p->next->grounding;tp=p;
                }p=p->next;
            }
            if (tp!=NULL) {
                p2=current->head;//保存要进行替换的位置
                p1=tp->next;
                tp->next=tp->next->next;//链表前驱 连接 链表后续
                current->head=p1;//开始替换移位
                p1->next=p2;
            }
            op=current->head;
            for (int i=1; i<current->total-1; i++) {
                compare=op->next->grounding;tp=NULL;
                p=op->next;
                for (int j=i; j<current->total-1; j++) {
                    if (compare>p->next->grounding) {
                        compare=p->next->grounding;tp=p;
                    }p=p->next;
                }
                if (tp!=NULL) {
                    p2=op->next;//保存要进行替换的位置
                    p1=tp->next;
                    tp->next=tp->next->next;//链表前驱 连接 链表后续
                    op->next=p1;//开始替换移位
                    p1->next=p2;
                }
                op=op->next;//跳入下一个数据位置
            }}
            break;

        default:
            cout<<"发生错误code:1.sort"<<endl;sleep(1);
            return false;
    }
    current=NULL;
    cout<<"排序完成！"<<endl;sleep(1);
    return true;
}
bool SellerManage::Display(){
    cout<<"探针:"<<" 总:"<<total<<" 头针:"<<head<<" 尾针:"<<tail<<endl;
    Seller *p=head;
    for(int i=0;i<total;i++){
        cout<<"序号:"<<i+1<<"ID:"<<p->id<<" name:"<<p->name<<" 密码:"<<p->passwords<<" 电话:"<<p->tel<<" 地址:"<<p->address<<" 在售商品数:"<<p->total<<endl;
        p=p->next;
    }
    return true;}
bool SellerManage::DebugMenu(){
    int type;
    while(1){
        cout<<"欢迎"<<endl;
        cout<<"1.显示所有商品信息"<<endl;
        cout<<"2.显示所有买家信息"<<endl;
        cout<<"3.显示所有卖家信息"<<endl;
        cout<<"4.查看卖家指针"<<endl;
        cout<<"5.查看管理密码"<<endl;
        cout<<"6.修改管理密码"<<endl;
        cout<<"7.重置所有储存数据"<<endl;
        cout<<"0.返回"<<endl;
        cout<<"请输入:";cin>>type;
        switch (type) {
            case 1:
                Magegoo.Display();
                cout<<"请选择一个序号进行操作(0为返回):";cin>>type;
                switch (type) {
                    case 0:
                        cout<<"正在返回"<<endl;
                        break;
                    default:
                        Magegoo.Editer(type);
                        break;
                }sleep(1);
                break;
            case 2:
                Magebuy.Display();
                cout<<"请选择一个序号进行操作(0为返回):";cin>>type;
                switch (type) {
                    case 0:
                        cout<<"正在返回"<<endl;
                        break;
                    default:
                        Magebuy.Editer(type);
                        break;
                }sleep(1);
                break;
            case 3:
                Display();
                cout<<"请选择一个序号进行操作(0为返回):";cin>>type;
                switch (type) {
                    case 0:
                        cout<<"正在返回"<<endl;
                        break;
                    default:
                        Editer(type);
                        break;
                }sleep(1);
                break;
            case 4:
                cout<<"卖家:头"<<head<<"尾"<<tail<<"当前"<<current<<endl;
                break;
            case 5:
                Magegoo.Uppasswords(false);
                sleep(1);
                break;
            case 6:
                Magegoo.Uppasswords(true);
                sleep(1);
                break;
            case 7:
            {
                ofstream ofile;
                ofile.open(Goodsdata);
                ofile.close();
                ofile.open(Buyerdata);
                ofile.close();
                ofile.open(Sellerdata);
                ofile.close();
                
            }cout<<"重置完成,请重启生效"<<endl;
                sleep(1);
                break;
            default:
                cout<<"正在返回"<<endl;sleep(1);
                return true;
                
        }}
}
bool SellerManage::AllMenu(){
    int type=0;bool ret=false;string type2,type3;
    while (1) {//管理目录总循环
        cout<<"简易网上购物系统"<<endl;
        cout<<"1.查询商品"<<endl;
        cout<<"2.买家登陆"<<endl;
        cout<<"3.卖家登陆"<<endl;
        cout<<"4.买家账号申请"<<endl;
        cout<<"5.卖家账号申请"<<endl;
        cout<<"6.管理员入口"<<endl;
        cout<<"0.退出"<<endl;
        cout<<"请输入:"<<endl;
        cin>>type;
        switch (type) {
            case 1:{
                if(Magegoo.Seekmenu()){Magegoo.Show();}
                cout<<"输入任意键返回."<<endl;getchar();getchar();}
                cout<<"正在返回\n";
                ret=true;
                sleep(1);
                break;
            case 2:
                cout<<"请输入账号:";cin>>type2;
                cout<<"请输入密码:";cin>>type3;
                if(Magebuy.Confirm(type2, type2, type3))Magebuy.Menu(Magegoo);
                else{cout<<"账号或密码错误！"<<endl;sleep(1);}
                break;
            case 3:
                cout<<"请输入账号:";cin>>type2;
                cout<<"请输入密码:";cin>>type3;
                if(Confirm(type2, type2, type3))Menu();
                else{cout<<"账号或密码错误！"<<endl;sleep(1);}
                break;
            case 4:Magebuy.Apply();break;
            case 5:Apply();break;
            case 6:
                cout<<"请输入管理密码:";cin>>type2;
                if(Magegoo.AllConfirm(type2))
                    ret=Magegoo.Menu();else{cout<<"密码错误:"<<endl;sleep(1);}
                break;
            case 808:
                cout<<"请输入调试密码:";cin>>type2;
                ret=DebugMenu();
                break;
            case 0:
                cout<<"正在退出!"<<endl;sleep(1);
                return true;
                break;
            default:
                cout<<"输入错误！请重新输入:"<<endl;sleep(1);
                break;
        }
        if (ret==false) {
            //cout<<"程序运行出现错误!请联系我们修正。"<<endl;sleep(1);
        }
    }return true;
}
bool SellerManage::Show(){
    if (current==NULL) {
        cout<<"无定位"<<endl;
        return false;
    }
    Seller *p=current;
    cout<<"ID:"<<p->id<<" 名称:"<<p->name<<" 密码:"<<p->passwords<<" 电话:"<<p->tel<<" 地址:"<<p->address<<" 在售商品数:"<<p->total<<endl;
    return true;}

//--------------------------------------------------GoodsManage
bool GoodsManage::Add(){
        Goods *datanew=new Goods;
        cout<<"商品编号:";cin>>datanew->num;
        cout<<"名称:";cin>>datanew->name;
        cout<<"规格(长、宽、高):";cin>>datanew->spe.length>>datanew->spe.wide>>datanew->spe.height;
        cout<<"价格:";cin>>datanew->price;
        cout<<"库存:";cin>>datanew->volume;
        datanew->salevolume=0;
        cout<<"类型:";cin>>datanew->style;
        cout<<"产地:";cin>>datanew->producingplace;
        datanew->belong=supername;//商家
        datanew->grounding=Gettime();//上架日期
        datanew->next=NULL;
        if (total==0) {
            head=datanew;
        }else{
            tail->next=datanew;}
        tail=datanew;total++;
    current=tail;
    return true;
}
bool GoodsManage::Disvector(){//显示vector容器内容 for seek
    for (int i=0; i<vectGoods.size(); i++) {
        cout<<"序号:"<<i+1<<"商品编号:"<<vectGoods[i].num<<" 名称:"<<vectGoods[i].name<<" 规格:"<<vectGoods[i].spe.length<<"*"<<vectGoods[i].spe.wide<<"*"<<vectGoods[i].spe.height;
        cout<<" 价格:"<<vectGoods[i].price<<" 库存:"<<vectGoods[i].volume<<" 销量:"<<vectGoods[i].salevolume<<" 类型:"<<vectGoods[i].style<<" 产地:"<<vectGoods[i].producingplace<<" 商家:"<<vectGoods[i].belong<<" 上架日期:"<<vectGoods[i].grounding<<endl;
    }return true;
}
bool GoodsManage::Display(){//显示所有(suoyou)商品
    cout<<"探针:"<<" 总:"<<total<<" 头针"<<head<<" 尾针"<<tail<<endl;
    Goods *p=head;
    for(int i=0;i<total;i++){
        cout<<"序号:"<<i+1<<" 商品编号:"<<p->num<<" 名称:"<<p->name<<" 规格:"<<p->spe.length<<"*"<<p->spe.wide<<"*"<<p->spe.height;
        cout<<" 价格:"<<p->price<<" 库存:"<<p->volume<<" 销量:"<<p->salevolume<<" 类型:"<<p->style<<" 产地:"<<p->producingplace<<" 商家:"<<p->belong<<" 上架日期:"<<p->grounding<<endl;
        p=p->next;
    }
    return true;
}
bool GoodsManage::Show(){//显示当前定位商品
    if (current==NULL) {
        cout<<"无定位"<<endl;
        return false;
    }
    cout<<"商品编号:"<<current->num<<" 名称:"<<current->name<<" 规格:"<<current->spe.length<<"*"<<current->spe.wide<<"*"<<current->spe.height;
    cout<<" 价格:"<<current->price<<" 库存:"<<current->volume<<" 销量:"<<current->salevolume<<" 类型:"<<current->style<<" 产地:"<<current->producingplace<<" 商家:"<<current->belong<<" 上架日期:"<<current->grounding<<endl;return true;
}
bool GoodsManage::Seekmenu(){
    int type;string type3;double type2;
    while (1) {
    cout<<"欢迎来到智能搜索引擎"<<endl;
    cout<<"1.通过商品编号查找"<<endl;
    cout<<"2.通过商品名称查找"<<endl;
    cout<<"3.通过商品规格查找~缺省"<<endl;
    cout<<"4.通过商品价格查找"<<endl;
    cout<<"5.通过商品库存查找"<<endl;
    cout<<"6.通过商品销量查找"<<endl;
    cout<<"7.通过商品类型查找"<<endl;
    cout<<"8.通过商品产地查找"<<endl;
    cout<<"9.通过商品商家查找"<<endl;
    cout<<"10.通过商品上架日期查找"<<endl;
    cout<<"0.返回"<<endl;
        cout<<"请输入:";cin>>type;
        switch (type) {
            case 1:
            case 2:
                cout<<"请输入数据:";cin>>type3;
                if(Search(type3, 0, type)==false){
                    if(Vagueseek(type3, type)){
                        while(1){
                        Disvector();
                            cout<<"请确定商品编号(输入0返回):";cin>>type3;if(type3=="0")break;
                            if(Search(type3, 0, 1)==true){return true;
                            }else{cout<<"未找到该数据！"<<endl;sleep(1);}}}
                    else{cout<<"未找到该数据！"<<endl;sleep(1);}
                }else{
                    while(1){return true;}}
                break;
            case 7:
            case 8:
            case 9:
            case 10:
                cout<<"请输入数据:";cin>>type3;
                if(Search(type3,0, type)==false){
                    if(Filter(type3,0,type)){
                        while(1){
                            Disvector();
                            cout<<"请确定商品编号(输入0返回):";cin>>type3;if(type3=="0")break;
                            if(Search(type3, 0, 1)==true){return true;//Editer();
                            }else{cout<<"未找到该数据！"<<endl;sleep(1);}}}
                    else{cout<<"未找到该数据！"<<endl;sleep(1);}
                }else{
                    while(1){return true;}}
                break;
            case 4:
            case 5:
            case 6:
                cout<<"请输入数据:";cin>>type2;
                if(Search("",type2, type)==false){
                    if(Filter("",type2,type)){while(1){
                        Disvector();
                        cout<<"请确定商品编号(输入0返回):";cin>>type3;if(type3=="0")break;
                        if(Search(type3, 0, 1)==true){return true;
                        }else{cout<<"未找到该数据！"<<endl;sleep(1);}}}
                else{cout<<"未找到该数据！"<<endl;sleep(1);}
        }else{
            while(1){return true;}}
                break;
            default:
                cout<<"正在返回"<<endl;sleep(1);
                return false;
        }
    return false;
    }
}
bool GoodsManage::AllConfirm(string passwords){//管理认证
    if (this->passwords==passwords||this->passwords.length()==0) {
        supername="管理员";return true;
    }else
        return false;
}
bool GoodsManage::Search(string sname,double dname,int style){
    Goods *p=head;
    switch (style) {
        case 0:
            for (int i=0; i<dname-1; i++) {
                p=p->next;
            }current=p;
            break;
        case 1:
            for (int i=0; i<total; i++) {
                if (p->num==sname)break;
                else p=p->next;
            }
            break;
        case 2:
            for (int i=0; i<total; i++) {
                if (p->name==sname)break;
                else p=p->next;
            }
            break;
        case 3:
            break;
        case 4:
            for (int i=0; i<total; i++) {
                if (p->price==dname)break;
                else p=p->next;
            }
            break;
        case 5:
            for (int i=0; i<total; i++) {
                if (p->volume==(int)dname)break;
                else p=p->next;
            }
            break;
        case 6:
            for (int i=0; i<total; i++) {
                if (p->salevolume==(int)dname)break;
                else p=p->next;
            }
            break;
        case 7:
            for (int i=0; i<total; i++) {
                if (p->style==sname)break;
                else p=p->next;
            }
            break;
        case 8:
            for (int i=0; i<total; i++) {
                if (p->producingplace==sname)break;
                else p=p->next;
            }
            break;
        case 9:
            for (int i=0; i<total; i++) {
                if (p->belong==sname)break;
                else p=p->next;
            }
            break;
        case 10:
            for (int i=0; i<total; i++) {
                if (p->grounding==sname)break;
                else p=p->next;
            }
            break;
        default:cout<<"Error code:999 \n";
            break;
    }
    if(p!=NULL){current=p;cout<<"找到此商品"<<endl;sleep(1);return true;}
    else {current=NULL;cout<<"未找到此商品"<<endl;sleep(1);return false;}
}
bool GoodsManage::Editstatus(double change,int style){
    switch (style) {
        case 4:
            if (change<0&&(-change)>current->price)return false;
            current->price+=change;
            break;
        case 5:
            if (change<0&&(-change)>current->volume)return false;
            current->volume+=change;
            break;
        case 6:
            if (change<0&&(-change)>current->salevolume)return false;
            current->salevolume+=change;
            break;
        default:cout<<"无此项！"<<endl;sleep(1);
            return false;
    }
    return true;
}
bool GoodsManage::Delete(int num){
    Goods *p=head,*p2;
    switch (num) {
        case 0:
        {
            if (p==current) {
                total--;
                if(total==0){delete p;head=tail=NULL;}
                else {p2=p;head=p->next;delete p2;}
                current=NULL;return true;
            }else
                for (int i=0; i<total-1; i++) {
                    if (p->next==current){break;}
                    else p=p->next;
                }
        }
            break;
            
        default:
        {
            if (num==1) {//判断是否为首个
                total--;
                if(total==0){delete head;tail=head=NULL;}//判断是否只有一个
                else {p2=p;head=p->next;delete p2;}
                current=NULL;return true;
            }else
                for (int i=0; i<num-2; i++) {
                    p=p->next;
                }
        }
            break;
    }
    p2=p->next;total--;
    if (p->next->next!=NULL) {//判断是否为尾巴
        p->next=p->next->next;
        current=NULL;
    }else {p->next=NULL;tail=p;}
    delete p2;
    return true;
}
bool GoodsManage::Uppasswords(bool style){
    if (style) {
    string temp1,temp2;
    cout<<"请输入你要修改的密码:";cin>>temp1;
    cout<<"请再次输入以确定此密码:";cin>>temp2;
    if (temp1==temp2) {
        passwords=temp2;cout<<"修改成功"<<endl;return true;
    }else{cout<<"修改失败"<<endl;return false;}
    }else{cout<<"当前密码:"<<passwords<<"(null?)"<<endl;return true;}
}
bool GoodsManage::Menu(){  //管理目录
    int type;string temp1,temp2;
    while (1) {
        cout<<"欢迎:管理员"<<endl;
        cout<<"当前商品量:"<<total<<endl;
        cout<<"1.查看所有商品\n";
        cout<<"2.查找商品\n";
        cout<<"3.添加商品\n";
        cout<<"4.删除商品\n";
        cout<<"5.修改商品信息\n";
        cout<<"6.购买该商品\n";
        cout<<"7.补充该商品\n";
        cout<<"8.显示当前定位商品\n";
        cout<<"9.修改管理员密码\n";
        cout<<"10.读取商品数据\n";
        cout<<"11.保存商品数据\n";
        cout<<"0.返回\n";
        cin>>type;
        switch (type) {
            case 1:{
                cout<<"所有商品:\n";
                Display();
                cout<<"你对第几个商品进行操作？\n";
                cout<<"0.返回\n";
                cin>>type;
                if (type==0) {
                    cout<<"正在返回"<<endl;sleep(1);
                    break;
                }else {if(type<=total&&type>0){if(Search("", type, 0))Editer(0);}else cout<<"无此项"<<endl;}
            }sleep(2);
                break;
            case 2:
                if(Seekmenu()){Editer(0);}
                sleep(1);
                break;
            case 3:{
                if(Add()==true){cout<<"添加成功！"<<endl;sleep(1);}else{cout<<"添加失败！"<<endl;}}
                break;
            case 4:{
                cout<<"请输入编号:";cin>>temp1;
                if(Search(temp1, 0, 1)==true){
                    Show();
                    if(Delete(0)==true)cout<<"删除成功!"<<endl;else cout<<"删除失败!"<<endl;}
                sleep(1);}
                break;
            case 5:
                cout<<"请输入编号:";cin>>temp1;
                if(Search(temp1, 0, 1)==true){
                    
                    Editer(0);}else{cout<<"无此商品！"<<endl;sleep(1);}
                sleep(1);
                break;
            case 6:
                cout<<"请输入编号:";cin>>temp1;
                if(Search(temp1, 0, 1)==true){
                    Show();
                cout<<"请输入购买数量:";cin>>type;
                    if(Editstatus((-type), 5)){Editstatus(type, 6);cout<<"操作完成!"<<endl;}else{cout<<"操作失败！"<<endl;}}
                sleep(1);
                break;
            case 7:
                cout<<"请输入编号:";cin>>temp1;
                if(Search(temp1, 0, 1)==true){
                    Show();
                cout<<"请输入补充数量:";cin>>type;
                    Editstatus(type, 5);cout<<"操作完成!"<<endl;}
                sleep(1);
                break;
            case 8:
                if(current!=NULL){Show();sleep(1);}else{cout<<"暂无定位。"<<endl;}
                sleep(1);
                break;
            case 9:
                Uppasswords(true);
                sleep(1);
                break;
            case 10:
                Load();
                sleep(1);
                break;
            case 11:
                Save();
                sleep(1);
                break;
            default:
                cout<<"正在返回"<<endl;sleep(1);
                return true;
        }
    }return true;}
bool GoodsManage::Vagueseek(string sname,int style){
    Goods *p=head;vectGoods.clear();
    switch (style) {
        case 1:
            for (int i=0; i<total; i++) {
                if(p->num.find(sname) != string::npos)
                    vectGoods.push_back(*p);
                    p=p->next;
            }
            break;
        case 2:
            for (int i=0; i<total; i++) {
                if(p->name.find(sname) != string::npos)
                    vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 7:
            for (int i=0; i<total; i++) {
                if(p->style.find(sname) != string::npos)
                    vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 8:
            for (int i=0; i<total; i++) {
                if(p->producingplace.find(sname) != string::npos)
                    vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 9:
            for (int i=0; i<total; i++) {
                if(p->belong.find(sname) != string::npos)
                    vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 10:
            for (int i=0; i<total; i++) {
                if(p->grounding.find(sname) != string::npos)
                    vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        default:cout<<"Error code:888 \n";
            break;
    }
    if(vectGoods.empty()==false){cout<<"找到此关键字商品"<<endl;sleep(1);return true;}
    else {cout<<"未找到此关键字商品"<<endl;sleep(1);return false;}
}
bool GoodsManage::Filter(string sname,double dname,int style){
    vectGoods.clear();
    Goods *p=head;
    switch (style) {
        case 4:
            for (int i=0; i<total; i++) {
                if (p->price==dname)vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 5:
            for (int i=0; i<total; i++) {
                if (p->volume==(int)dname)vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 6:
            for (int i=0; i<total; i++) {
                if (p->salevolume==(int)dname)vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 7:
            for (int i=0; i<total; i++) {
                if (p->style==sname)vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 8:
            for (int i=0; i<total; i++) {
                if (p->producingplace==sname)vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 9:
            for (int i=0; i<total; i++) {
                if (p->belong==sname)vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        case 10:
            for (int i=0; i<total; i++) {
                if (p->grounding==sname)vectGoods.push_back(*p);
                p=p->next;
            }
            break;
        default:cout<<"Error code:999 \n";
            break;
    }
    if(vectGoods.empty()==false){cout<<"找到此关键字商品"<<endl;sleep(1);return true;}
    else {cout<<"未找到此关键字商品"<<endl;sleep(1);return false;}
}
bool GoodsManage::Load(){
    Goods *p,*p2=NULL;
    ifstream ifile;
    ifile.open(Goodsdata);
    if (!ifile){ifile.close();cout<<"&商品数据导入失败"<<endl;sleep(1);return false;}//if(!ifile.eof())判断是非读结束
    char temp;
    while (ifile.get(temp)){
        if (temp == ':') break;
    }
    ifile >> total>>passwords;
    if (passwords=="admin") {
        passwords="";
    }
    if (total<=0){ifile.close();return false;}
    for (int i = 0; i < total; i++)
    {
        if (i==0) {
            p=head=new Goods;
        }else{p2->next=p=new Goods;}
        ifile>>p->num>>p->name>>p->spe.length>>p->spe.wide>>p->spe.height>>p->price>>p->volume>>p->salevolume;
        ifile>>p->style>>p->producingplace>>p->belong>>p->grounding;
        tail=p2=p;//该数据条倒入完成
    }
    ifile.close();cout<<"&商品数据导入成功"<<endl;sleep(1);
    return true;
}
bool GoodsManage::Save(){
    Goods *p=head;
    fstream ofile;
    ofile.open(Goodsdata,ios::out);
    if (passwords.length()==0) {
        passwords="admin";
    }
    ofile <<"此文件为商品数据库DataVolume:"<<total<<'\t' <<passwords<<endl;
    for (int i = 0; i < total; i++)
    {
        ofile <<p->num<<'\t'<<p->name<<'\t' <<p->spe.length<<'\t' <<p->spe.wide<<'\t' <<p->spe.height<<'\t' <<p->price<<'\t' <<p->volume<<'\t' <<p->salevolume<<'\t' ;
        ofile <<p->style<<'\t' <<p->producingplace<<'\t' <<p->belong<<'\t' <<p->grounding<<endl;
        p=p->next;
    }
    ofile.close();
    return true;
}
bool GoodsManage::Editer(int style){
    int type2;string temp1;Goods *p=head;
    if (style>0) {
        if(style>total){cout<<"错误范围"<<endl;return false;}
        for (int i=0; i<style-1; i++) {
            p=p->next;
        }current=p;style=0;
    }else if(style<0){cout<<"错误范围"<<endl;return false;}
    if (style==0) {
    double type;
    while(1){
        Show();
        cout<<"1.修改该商品编号"<<endl;
        cout<<"2.修改该商品名称"<<endl;
        cout<<"3.修改该商品规格＊3"<<endl;
        cout<<"4.修改该商品价格"<<endl;
        cout<<"5.修改该商品库存"<<endl;
        cout<<"6.修改该商品销量"<<endl;
        cout<<"7.修改该商品类型"<<endl;
        cout<<"8.修改该商品产地"<<endl;
        cout<<"9.修改该商品商家"<<endl;
        cout<<"10.修改该商品上架时间"<<endl;
        cout<<"11.删除该商品数据"<<endl;
        cout<<"0.返回"<<endl;
        cout<<"请输入:"<<endl;cin>>type2;
        switch (type2) {
            case 1:
            case 2:
            case 7:
            case 8:
            case 9:
            case 10:
                cout<<"请输入你要修改后的数据:"<<endl;
                cin>>temp1;
                Update(temp1, type2);
                break;
            case 3:
                cout<<"请输入你要修改后的数据:x3"<<endl;
                cin>>current->spe.length>>current->spe.wide>>current->spe.height;
                cout<<"修改成功！"<<endl;sleep(1);
                break;
            case 4:
            case 5:
            case 6:
                cout<<"请输入你要改变的量:"<<endl;
                cin>>type;
                Editstatus(type, type2);
                break;
            case 11:
                if(Delete(0)){cout<<"删除成功！"<<endl;}
                return true;
            default:cout<<"正在返回！"<<endl;sleep(1);
                return false;
        }
    }}
    return true;
}
bool GoodsManage::Update(string type, int style){
    Goods *p=current;
    switch (style) {
        case 1:p->num=type;
            break;
        case 2:p->name=type;
            break;
        case 7:p->style=type;
            break;
        case 8:p->producingplace=type;
            break;
        case 9:p->belong=type;
            break;
        case 10:p->grounding=type;
            break;
        default:cout<<"发生错误code:1.update \n";sleep(1);
            return false;
    }
    cout<<"修改成功！"<<endl;sleep(1);
    return true;
}
int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    SellerManage Mage;//启动程序
    return 0;
}
