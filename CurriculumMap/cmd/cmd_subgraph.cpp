#include "cmd_Point.h"
#include "cmd_LogicP.h"
#include "cmd_Course.h"
#include "global.h"

#include<queue>
#include<cassert>
#include<algorithm>
#include<cstdio>

void distribute(vector<PointX*> subgraph,vector<Block*> block_list)
{
    vector<Course*> A[20];
    vector<LogicP*> B[20];
    //1 basic layer distribution
    for(auto x:subgraph)
    {
        int mx=-1;x->degree=0;
        for(auto y:x->get_from()) if(y->visible) x->degree++,mx=max(mx, y->layer );
        for(auto y:x->get_to()) if(y->visible) x->degree++;
        x->layer = mx+x->layer_val();
        if(mx<0) assert(x->layer_val());
    }
    //2 move backward to reduce size of layer 0
    for(int i=(int)subgraph.size()-1;i>=0;i--)
    {
        int mi=100;
        for(auto y:subgraph[i]->get_to()) if(y->visible) mi=min(mi, y->layer - y->layer_val() );
        if(mi!=100) subgraph[i]->layer=mi;
    }
    //3 move course in block together
    for(auto b:block_list)
    {
        int mx=0;for(auto x:b->in) mx=max(mx,x->layer);
        for(auto x:b->in) x->layer=mx;
    }
    for(auto x:subgraph)
        if(x->layer_val()==0)//4 move forward logic PointX to make it closer to course PointX
        {
            int mx=-1;
            for(auto y:x->get_from()) if(y->visible)  mx=max(mx, y->layer );
            x->layer = mx+x->layer_val();
        }
        else if(x->v1 or x->v2) //5 consider single course but needed by co/exclu from originally visible course
        {
            Course* xx=static_cast<Course*>(x);
            if(Global::Want_exclu) for(auto y:xx->get_exclu()) if(y->degree==0 and y->belong==0) y->layer=x->layer;
            if(Global::Want_co) for(auto y:xx->get_co()) if(y->degree==0 and y->belong==0) y->layer=x->layer;
        }

    for(auto x:subgraph)
        if(x->layer_val())
            A[x->layer].push_back(static_cast<Course*>(x));
        else
            B[x->layer].push_back(static_cast<LogicP*>(x));

    Global::Pos now={0,0};//spare space for exclusion arrow
    const int Width=Global::button_width+100, Height=Global::button_height+30;
    int H=0,max_H=0;
    for(int i=0,depth=0;i<20;i++,depth++) if(A[i].size()) //i=layer
    {
        stable_sort(A[i].begin(),A[i].end(), [&](Course* a,Course* b){
            if(a->belong!=b->belong)//for merging block
            {
                if(a->belong==nullptr or b->belong==nullptr) return a->belong > b->belong;
                return a->belong->in[0]->get_name()<b->belong->in[0]->get_name();
            }
            int aa= (a->get_from().empty() or a->get_from()[0]->visible==0 ? 0 : a->get_from()[0]->degree );
            int bb= (b->get_from().empty() or b->get_from()[0]->visible==0 ? 0 : b->get_from()[0]->degree );
            if(aa*bb>0) return aa<bb;
            if(a->degree!=b->degree) return a->degree>b->degree;//lonely course go to bottom
            return a->get_name()<b->get_name();//neither reference object
        } );
        now.second=0;int H1=max( (int) (H/ (A[i].size()+1) ),Height);
        for(auto x:A[i])
        {
            now.second+=H1,
            Global::Course_list.push_back({x,now}),
            x->depth=depth;
            max_H=max(max_H,now.second);
            if(x->degree>0) H=max(H,now.second);//lonely course don't contribute to max H
            x->degree=now.second;//for logic point
        }
        //write the same rectangle more than once is acceptable
        for(auto t:A[i]) if(t->belong!=nullptr)
        {
            int mi=now.second,mx=0;
            for(auto x:t->belong->in) mi=min(mi,x->degree),mx=max(mx,x->degree);
            mx+=Global::button_height;
            Global::Rectangle_list.push_back({{now.first-10,mi-10},{now.first+Global::button_width+10,mx+10}});
        }
        now.first+=Width;

        //calculate depth for logic PointX
        for(auto x:B[i])
        {
            int mx=0,sum=0,cnt=0;
            for(auto y:x->get_from()) if(y->visible)  mx=max(mx, y->depth ),cnt++,sum+=y->degree;
            x->depth = mx+1;depth=max(depth,x->depth);
            assert(cnt>0);x->degree=sum/cnt;
        }
        stable_sort(B[i].begin(),B[i].end(), [&](LogicP* a,LogicP* b){
            return a->depth<b->depth or (a->depth==b->depth and a->degree<b->degree) ;
        } );
        for(int l=0,r;l<(int)B[i].size();l=r+1)
        {
            r=l;while(r+1<(int)B[i].size() and B[i][l]->depth==B[i][r+1]->depth) r++;
            now.second=0;int H2=max( (int) (H/(r-l+2)),Height),sum=0;
            for(int k=l;k<=r;k++) sum+=H2,B[i][k]->degree=(sum*3+B[i][k]->degree*1)/4;
            int mid=(l+r)/2;
            for(int k=mid-1;k>=l;k--) B[i][k]->degree=min(B[i][k]->degree,B[i][k+1]->degree-Height);
            for(int k=mid+1;k<=r;k++) B[i][k]->degree=max(B[i][k]->degree,B[i][k-1]->degree+Height);
            for(int k=l;k<=r;k++) Global::LogicP_list.push_back({B[i][k],{now.first,B[i][k]->degree}});
            now.first+=Width*0.6;
        }
    }
    Global::Graph_width=now.first+Global::button_width,
    Global::Graph_height=max_H+Global::button_height;
}

namespace Global{
//top sort
void prepare()
{
    unsigned int total=Global::Allpt.size();
    for(auto x:Global::Allpt) for(auto y:x->get_to()) y->degree++;
    queue<PointX*> q;for(auto x:Global::Allpt) if(x->degree==0) q.push(x),x->depth=1;
    Global::Allpt.clear();
    while(!q.empty())
    {
        PointX *x=q.front();q.pop();Global::Allpt.push_back(x);
        for(auto y:x->get_to())
        {
            y->degree--;
            if(!y->degree) q.push(y),y->depth=x->depth+1;
        }
    }
    assert(total==Global::Allpt.size());
}

//extract subgraph
void extract()
{
    Global::Course_list.clear();
    Global::LogicP_list.clear();
    Global::Edge_list.clear();
    Global::Exclu_list.clear();
    Global::Coreq_list.clear();
    Global::Rectangle_list.clear();

    //build subset of point
    for(auto x:Global::Allpt) x->v1=x->v2=0,x->v3=1;
    for(auto x:Global::Allpt) if(x->get_enrolled())
        for(auto z:x->get_to()) if(z->get_name()=="OR")
            for(auto y:z->get_from()) y->v3=0;//v3->no further for y's pre-requisite
    for(unsigned int i=Global::Allpt.size();i>0;i--)
    {
        PointX *x=Global::Allpt[i-1];
        x->v1|=x->get_searched();//v1->want to further ask
        if(x->v1 and x->v3)
        {
            bool satisfied=0;
            if(x->get_name()=="OR") for(auto y:x->get_from()) satisfied|= y->get_enrolled();
            if(!satisfied) for(auto y:x->get_from()) y->v1=1;
        }
    }
    for(auto x:Global::Allpt)
    {
        x->v2|=x->get_r_searched();
        if(x->v2) for(auto y:x->get_to()) y->v2=1;
    }
    for(auto x:Global::Allpt) x->visible= (x->v1 or x->v2 or x->get_enrolled());
    for(auto x:Global::Allpt) if( (x->v1 or x->v2 or x->get_enrolled()) and x->layer_val())//originally visible (one more step search)
    {
        Course* xx=static_cast<Course*>(x);
        if(Global::Want_exclu) for(auto y:xx->get_exclu()) y->visible=1;
        if(Global::Want_co) for(auto y:xx->get_co()) y->visible=1,Global::Coreq_list.push_back({y,xx});
    }

    //merge course that exclude each other

    //beauty of brute force:
    //general solution to merge larger complete subgraph could be given but not related what we learned
    //so just keep easy to understand since this kind of cases are really limited (basically all 3+ are here)
    Block* b[12];vector<Block*> block_list;
    for(int i=0;i<12;i++) block_list.push_back(b[i]=new Block);
    for(auto x:Global::Allpt) if(x->visible and x->layer_val())//a course
    {
        Course* xx=static_cast<Course*>(x);xx->belong=nullptr;
        if(xx->get_name()=="MATH 1003" or xx->get_name()=="MATH 1012" or xx->get_name()=="MATH 1013" or xx->get_name()=="MATH 1023") b[0]->ins(xx);
        if(xx->get_name()=="MATH 1014" or xx->get_name()=="MATH 1024" or xx->get_name()=="MATH 1020") b[1]->ins(xx);
        if(xx->get_name()=="MATH 2111" or xx->get_name()=="MATH 2121" or xx->get_name()=="MATH 2131") b[2]->ins(xx);
        if(xx->get_name()=="MATH 2351" or xx->get_name()=="MATH 2352" or xx->get_name()=="PHYS 2124") b[3]->ins(xx);
        if(xx->get_name()=="COMP 2711" or xx->get_name()=="COMP 2711H" or xx->get_name()=="MATH 2343") b[4]->ins(xx);
        if(xx->get_name()=="MATH 2411" or xx->get_name()=="LIFS 3150" or xx->get_name()=="IEDA 2510") b[5]->ins(xx);
        if(xx->get_name()=="ELEC 2600" or xx->get_name()=="ELEC 2600H") b[6]->ins(xx);
        if(xx->get_name()=="MATH 4991" or xx->get_name()=="MATH 4992" or xx->get_name()=="MATH 4993" or xx->get_name()=="MATH 4994" or xx->get_name()=="MATH 4999") b[7]->ins(xx);
        if(xx->get_name()=="ELEC 4130" or xx->get_name()=="COMP 4421" or xx->get_name()=="MATH 4336") b[8]->ins(xx);
        if(xx->get_name()=="CENG 4920" or xx->get_name()=="CENG 4930" or xx->get_name()=="CENG 4940") b[8]->ins(xx);
        if(xx->get_name()=="BIEN 4920" or xx->get_name()=="BIEN 4930" or xx->get_name()=="BIEN 4940") b[9]->ins(xx);
        if(xx->get_name()=="PHYS 1111" or xx->get_name()=="PHYS 1112" or xx->get_name()=="PHYS 1312") b[10]->ins(xx);
        if(xx->get_name()=="ISOM 3260" or xx->get_name()=="IEDA 3300" or xx->get_name()=="COMP 3311") b[11]->ins(xx);
        xx->bi_exclu.clear();
        for(auto y:xx->get_exclu()) if(y->visible and y->get_exclu().count(xx)) xx->bi_exclu.push_back(y);
    }
    for(int i=0;i<12;i++) if(b[i]->in.size()==1) b[i]->in[0]->belong=nullptr,b[i]->in.clear();//release
    for(auto x:Global::Allpt) if(x->visible and x->layer_val())
    {
        Course* xx=static_cast<Course*>(x);
        if(xx->belong!=nullptr or xx->bi_exclu.size()!=1) continue;
        Course* yy=xx->bi_exclu[0];if(yy->belong!=nullptr) continue;
        Block *b=new Block;block_list.push_back(b);
        b->ins(xx),b->ins(yy);
    }
    //collect
    vector<PointX*> subgraph;
    for(auto x:Global::Allpt) if(x->visible) subgraph.push_back(x);
    distribute(subgraph,block_list);

    if(Global::Want_pre) for(auto t:Global::Course_list)
    {
        Course* x=t.first;
        for(auto y:x->get_from()) if(y->visible) Global::Edge_list.push_back({y,x});
    }
    if(Global::Want_pre) for(auto t:Global::LogicP_list) for(auto y:t.first->get_from()) if(y->visible) Global::Edge_list.push_back({y,t.first});
    //Exclu
    for(auto x:Global::Allpt) if(x->visible and x->layer_val())
    {
        Course* xx=static_cast<Course*>(x);
        if(Global::Want_exclu) for(auto y:xx->get_exclu()) if(y->visible)
            if(xx->belong==nullptr or xx->belong!=y->belong)//not in same block
                if(y->get_exclu().count(xx))//temp;
                        Global::Exclu_list.push_back({y,xx});
    }

    for(auto x:block_list)//memory release
    {
        for(auto y:x->in) y->belong=nullptr;
        delete x;
    }
}
}
