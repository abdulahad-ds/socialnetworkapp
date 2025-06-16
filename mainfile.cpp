#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;
class date;
class base;
class post;
class user;
class comment;
class memory;
class page;
class user;
class Activity;
class SocialNetworkApp;

class date {
    int day, month, year;

    static int getLastDayOfMonth(int m, int y) {
        switch (m) {
        case 2:
            return isLeapYear(y) ? 29 : 28;
        case 4: case 6: case 9: case 11:
            return 30;
        default:
            return 31;
        }
    }

    static bool isLeapYear(int y) {
        return (y % 400 == 0) || (y % 100 != 0 && y % 4 == 0);
    }

public:
    date(): day(1),month(1),year(2025){}
    date(int d, int m, int y) :day(d), month(m), year(y) {}

    bool isWithin24Hours(const date& systemDate) const
    {
        //same day
        if (day == systemDate.day && month == systemDate.month && year == systemDate.year)
        {
            return 1;
        }
        //previous day
        if (day == systemDate.day - 1 && month == systemDate.month && year == systemDate.year)
        {
            return 1;
        }
        //month boundary (next month 1st day vs prevmonth last day)
        if (systemDate.day == 1)
        {
            if (month == systemDate.month - 1 || (systemDate.month == 1 && month == 12) && year == systemDate.year)
            {
                if (month == 2)
                {
                    bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
                    int lastDayFeb = isLeap ? 29 : 28;
                    if (day == lastDayFeb)
                    {
                        return 1;
                    }
                }
                else if (month == 4 || month == 6 || month == 9 || month == 11)
                {
                    if (day == 30)
                    {
                        return 1;
                    }
                }
                else
                {
                    if (day == 31)
                    {
                        return 1;
                    }
                }
            }
            //year boundary(jan 1 vs dec 31(last year))
            else if (systemDate.month == 1 && month == 12 && year == systemDate.year - 1)
            {
                if (day == 31)
                {
                    return 1;
                }
            }
        }
        return 0;
    }
string getFormattedDate() const
    {
        const char* monthNames[] = { "", "January", "February", "March", "April", "May", "June",
                                   "July", "August", "September", "October", "November", "December" };
        return to_string(day) + " " + monthNames[month];
    }
    date getPreviousDate() const {
        int prevDay = day - 1;
        int prevMonth = month;
        int prevYear = year;

        if (prevDay < 1)
        {
            prevMonth--;

            if (prevMonth < 1)
            {
                prevMonth = 12;
                prevYear--;
            }
            prevDay = getLastDayOfMonth(prevMonth, prevYear);
        }

        return date(prevDay, prevMonth, prevYear);
    }

    static int getCurrentYear()
    {
        return 2025;
    }
    int getDay()const
    {
        return day;
    }
    int getMonth() const
    {
        return month;
    }
    int getYear() const
    {
        return year;
    }
    void print() const
    {
        cout << day << "/" << month << "/" << year;
    }

};

class base {
protected:
    int id;
public:
    base(int i) :id(i) {}
    int getID() {
        return id;
    }
    virtual ~base() {}
};

class user : public base {
    string name;
    static int count;
    user** friends; //aggregation
    page** likedPages; //aggregation
    int friendsCount;
    int pagesCount;
    post** posts;
    int postCount;
public:

    user(int i, string n) :
        base(i), name(n), friends(nullptr), likedPages(nullptr), posts(nullptr), postCount(0),friendsCount(0), pagesCount(0) 
    {}
    
    ~user();

    //getters
    string getName(){
        return name;
    }
    static int getCount() {
        return count;
    }
    post** getPosts(){
        return posts;
    }
    static void incrementCount() {
        count++;
    }
    int getpostCount(){
        return postCount;
    }
    
    void setfriends(user** all, int* arr, int c) ;
    void setPages(page** all, int* arr, int c) ;
    bool istimeline(post*ptr);
    bool isFriend(user* ptr);
    bool isLiked(page* ptr);
    void viewUserPosts(const date& currentDate) const ;
    void viewHome(const date& currentDate);
    void addPost(int i, int d, int m, int y, int activityNum, string actValue,
        string text, int* likedBy, int lCount, SocialNetworkApp* ptr, string commentstr);
    void addFriend(user* ptr);
    void addLikePage(page* ptr);
    void addMemory(int i, string t, int d, int m, int y, int pID,
         SocialNetworkApp* ptr, int* likedBy, int lCount, string commentstr);
    void sortPosts();
    void viewTimeline()const;
    void viewMemories()const;
    friend post* findPostU(int id,  user** u,page **p);
    void viewLikedPagesList(page** allPages) const;
    void viewFriendsList(user** allUsers) const ;
    void viewAllUsers(user** allUsers, int &index);
    friend class SocialNetworkApp;

};

class page : public base {
    string name;
    user** likedBy; 
    post** posts;
    static int count;
    int likeCount;
    int postCount;
public:
    page(int n, string t) :base(n), name(t) {
        likeCount = 0;
        postCount = 0;
        posts = nullptr;
        likedBy = nullptr;
    }
     ~page();

    //getters
    static int getCount() 
    {
        return count;
    }
    static void incrementCount() {
        count++;
    }
    string getName()
    {
        return name;
    }
    post** getposts(){
        return posts;
    }
    int getPostCount(){
        return postCount;
    }

    friend class SocialNetworkApp;
    void setLikedy(user** all, int* arr, int c);
    void viewPosts(const date& currentDate) const;
    void viewPage() const;
    void sortPosts();
    friend post* findPostP(int id,page **p);
    void display();
    void addPost(int i, int d, int m, int y, int activityNum, string actValue, string text, int* likedBy, int lCount, SocialNetworkApp* ptr, string commentstr);
};

class Activity {
    int type;
    string value;

public:
    Activity(int t, string v) {
        type = t;
        value = v;
    }
    Activity() {
        type = 0;
        value = "";
    }
    int getNum() {
        return type;
    }
    string getValue() {
        return value;
    }
    void printActivity() const
    {

        if (type == 1) {
            cout << "is feeling " << value ;
        }
        else if (type == 2) {
            cout << "is thinking about " << value ;
        }
        else if (type == 3) {
            cout << "is making " << value ;
        }
        else if (type == 4)
            cout << "is celebrating " << value;
        else 
            cout << " shared ";
    }

};

class post : public base {
protected:
    string arr;
    string name;
    user** likedBy;
    comment** comments;
    int likeCount;
    int commentCount;
    date d;
    Activity* act; 
    user* ownerUser;
    page* ownerPage;
    friend class SocialNetworkApp;
public:

    post(int i, string a, date dt, user* u) :base(i), arr(a), d(dt), likedBy(nullptr), comments(nullptr),ownerUser(u),ownerPage(nullptr) {
        commentCount = 0;
        likeCount=0;
    }
    post(int i, string a, date dt, page* p) :base(i), arr(a), d(dt), likedBy(nullptr), comments(nullptr), ownerUser(nullptr), ownerPage(p) {
        commentCount = 0;
        likeCount=0;
    }
    post(int i, int d, int m, int y, post* ptr, user*u) :d(d, m, y), base(i) {
        ownerUser=u;
        ownerPage=nullptr;
        arr = ptr->getArr();
        this->act = ptr->getActivity();
        commentCount = 0;
        comments = nullptr;
        likedBy = nullptr;
        likeCount=0;
    }
    post(int i, int d, int m, int y, post* ptr, page*p) :d(d, m, y), base(i) {
        ownerUser=nullptr;
        ownerPage=p;
        arr = ptr->getArr();
        act = ptr->getActivity();
        commentCount = 0;
        comments = nullptr;
        likedBy = nullptr;
        likeCount=0;
    }
    post(int i, int d, int m, int y, int aNum, string aVal, string text, user* u) :base(i), d(d, m, y) {
        ownerUser=u;
        ownerPage=nullptr;
        arr = text;
        commentCount = 0;
        comments = nullptr;
        likedBy = nullptr;
        likeCount=0;
        if (aNum != 0) {

            act = new Activity(aNum, aVal);
        }
        else {
            act = nullptr;
        }

    }
    post(int i, int d, int m, int y, int aNum, string aVal, string text, page*p) :base(i), d(d, m, y) {
        ownerUser=nullptr;
        ownerPage=p;
        arr = text;
        commentCount = 0;
        comments = nullptr;
        likedBy = nullptr;
        likeCount=0;
        if (aNum == 0) {

            act = new Activity(aNum, aVal);
        }
        else {
            act = nullptr;
        }

    }
    ~post() ;
    //getters
    int getCommCount() {
        return commentCount;
    }
    int getID() {
        return id;
    }
    const date& getDate()const
    {
        return d;
    }
    Activity* getActivity()const
    {
        return act;
    }
    string getText()const
    {
        return arr;
    }
    int getCommentCount()const
    {
        return commentCount;
    }
    comment* getComment(int index) const
    {
        if (index < commentCount)
            return comments[index];
        return nullptr;
    }
    int getLikeCount(){
        return likeCount;
    }
    string getArr(){
        return arr;
    }
    user** getLikedby(){
        return likedBy;
    }

    
    bool isLiked(int uid);
    void setLikedby(user** u, page** p, int* arr, int c);
    void view();
    void addLike(user* current);
    void viewLikedByList(user** allUsers, page** allPages)const;
    void addComment(int id, int userID, string text, int d, int m, int y, SocialNetworkApp* ptr);
    virtual bool isMemory()const
    {
        return false;
    }
    virtual void displayMemory()const {}
};

class memory :public post {
    post* originalPost;
    string memCaption;
    date postDate;
public:
    memory(int i, int d, int m, int y, string caption, post* ptr,user*u):postDate(ptr->getDate()), post(i, d, m, y, ptr,u) {
        originalPost = ptr;
        memCaption = caption;
    }
    memory(int i, int d, int m, int y, string caption, post* ptr,page *p):postDate(ptr->getDate()), post(i, d, m, y, ptr,p) {
        originalPost = ptr;
        memCaption = caption;
    }
    ~memory(){}
    bool isMemory() const override
    {
        return true;
    }
    friend class SocialNetworkApp;
    void displayMemory() const;
};

class comment : public base {
    user* owneru; 
    page* ownerp;
    string arr;
    date d;
public:
    comment(int i, user* user, string a, date dt) :
        base(i), owneru(user), arr(a), d(dt)
    {ownerp= nullptr;}
    comment(int i, string text, int d, int m, int y, user* ptr) :d(d, m, y), base(i) {
        arr = text;
        owneru = ptr;
        ownerp = nullptr;
    }
    comment(int i, string text, int d, int m, int y, page* ptr) :d(d, m, y), base(i) {
        arr = text;
        ownerp = ptr;
        owneru = nullptr;
    }
    ~comment(){}
    void view() {
        cout << "\t\t" << arr << endl;
    }
    
    string getArr() const
    {
        return arr;
    }
    user* getUser()const
    {
        return owneru;
    }
    page* getPage()const
    {
        return ownerp;
    }
    date getDate()const
    {
        return d;
    }

};

int user::count = 0;
int page::count = 0;

class SocialNetworkApp {
private:
    user** u;
    page** p;
    date systemDate;
    friend class post;
    friend class page;
    friend class user;
    // reading
    static int countEntries(const string& filename);
    static int getNextId(const string& filename);
    static int getNextID2(const string& filename);
    static int readID(const string& filename, int num);
    static string readName(const string& filename, int num);
    static int* readCol3(const string& filename, int& size, int num);
    static int* readCol4(const string& filename, int& size, int num);
    void readComments(const string& comments, post* ptr);

    // creation functions
    void addPost(const string& line);
    void addPage(int id, const string& name);
    void addUser(int id, const string& name);
    void setMemory(const string& line);
    
    // File writing functions
    void writeNewMem(int mID, int pID, int uID,const  string& caption, const  date& d);
    void writeNewCom(int pID, const string &arr, int uID, int d, int m, int y);
    void writeLikePost(int id, int pID);
    void writeNewUser(const string& name, int id);
    void writeNewFriend(int id, int fID);
    void writeLikePage(int id, int pageID);
    
    // Utility functions
    user* getUserPtr(int id) const;
    page* getPagePtr(int id) const;
    post* findPostP(int id) const;
    post* findPostU(int id) const;
    
    
    // UI functions
    void viewPagesList();
    user* setUser() ;
    void commentOnPost(user* current);
    void viewLikes(user* current) ;
    void LikePost(user* current);
    void viewPostbyID(user* current);
    void addNewFriend(user* current);
    void home(user* current);
    void Timeline(user* current);
    void pageview(user* current, page* pageptr);
    void MainMenu(user* current);


public:
    SocialNetworkApp() : u(nullptr), p(nullptr), systemDate(14, 5, 2025) {}
    ~SocialNetworkApp(){

    for (int i = 0; i < user::getCount(); i++) {
        delete u[i];
    }
    delete[] u;
    
    for (int i = 0; i < page::getCount(); i++) {
        delete p[i];
    }
    delete[] p;
}
    void loadData();
    void run();
};
void SocialNetworkApp::readComments(const string& comments, post* ptr) {

    int commentCount = 0;
    int* commentsIDs = nullptr;
    if (comments != " "&& comments!="") {
        for (int i = 0; comments[i] != '\0'; i++) {
            if (comments[i] == ',')
                commentCount++;
        }
        commentCount++;
        commentsIDs = new int[commentCount];
        int index = 0;
        string temp = "";
        for (int i = 0; i < comments.size(); i++) {
            if (comments[i] == ' ')i++;
            while (comments[i] != ',' && i < comments.size()) {
                temp += comments[i];
                i++;
            };
            //cout << "4: "<< temp <<endl;
            commentsIDs[index++] = stoi(temp);
            temp = "";

        }
    }
    string filename = "comment.txt";
    int temp2 = countEntries(filename);
    string line;
    ifstream read(filename);

    for (int j = 0; j < commentCount; j++) {
        read.close();
        read.open(filename);
        for (int i = 0; i < temp2; i++) {

            getline(read, line);
            int sep = line.find('|');
            // cout << "5: "<< line.substr(0, sep) <<endl;
            int temp3 = stoi(line.substr(0, sep));

            if (temp3 == commentsIDs[j]) {

                //extract date
                int sep2 = line.find('|', sep + 1);


                string date = line.substr(sep + 1, sep2 - sep - 1);
                int s1 = date.find(',');
                int s2 = date.find(',', s1 + 1);
                
                int day = stoi(date.substr(0, s1));
                int month = stoi(date.substr(s1 + 1, s2 - s1 - 1));
                int year = stoi(date.substr(s2 + 1));

                //extract ownerID
                sep = sep2;
                sep2 = line.find('|', sep + 1);
                
                int OwnerID = stoi(line.substr(sep + 1, sep2 - sep - 1));

                //extract text
                sep = sep2;
                sep2 = line.find('|', sep + 1);
                string text = line.substr(sep + 1, sep2 - sep - 1);

                //add comment

                ptr->addComment(temp3, OwnerID, text, day, month, year, this);
                break;
            }
        }
    }
}

//ctrs(that require readComments)



//destructors
page:: ~page() {
        if (posts!=nullptr)
        {
            for (int i = 0; i < postCount; i++) {
                if (posts[i])
                {
                  delete posts[i];
                }
            }
            delete[] posts;
            posts=nullptr;
        }
        if (likedBy!=nullptr)
        {
            delete[] likedBy;
            likedBy=nullptr;
        }
    }
post:: ~post(){
        if (comments!=nullptr) {
            for (int i = 0; i < commentCount; i++) {
                if (comments[i]) delete comments[i];
            }
            delete[] comments;
            comments=nullptr;
        }

        if (likedBy!=nullptr)
        {
            delete[] likedBy;
            likedBy=nullptr;
        }
    }
user:: ~user(){
        if (posts!=nullptr) {
            for (int i = 0; i < postCount; i++) 
            {
                if (posts[i]!=nullptr) 
                delete posts[i];
                posts[i]=nullptr;
            }
            delete[] posts;
            posts=nullptr;
        }

        if (friends!=nullptr) 
        {
            delete[] friends;
            friends=nullptr;
        }
        if (likedPages!=nullptr)
        {
            delete[] likedPages;
            likedPages=nullptr;
        }
    }

//ids to ptr
user* SocialNetworkApp::getUserPtr( int id)const {
    for (int i = 0; i < user::getCount(); i++) {
        if (u[i]->getID() == id) {
            return u[i];
        }
    }
    return nullptr;
}
page* SocialNetworkApp::getPagePtr( int id)const {
    for (int i = 0; i < page::getCount(); i++) {
        if (p[i]->getID() == id) {
            return p[i];
        }
    }
    return nullptr;
}
post* SocialNetworkApp::findPostP(int id)const{
        for (int i=0; i<page::getCount();i++){
            post**posts=p[i]->getposts();
            for (int j=0; j<p[i]->getPostCount();j++){
            if (id==(posts[j])->getID()){
                return posts[j];
                
            }
        }
    }
        return nullptr;
}
post* SocialNetworkApp::findPostU(int id)const{
        post**posts=nullptr;
        for (int i=0; i<user::getCount();i++){
            posts=u[i]->getPosts();
            for (int j=0; j<u[i]->getpostCount();j++){
            if (id==(posts[j])->getID()){
                return posts[j];
            }
        }
        posts=nullptr;
        }
        return findPostP(id);
    }


//setters
void SocialNetworkApp::addPost(const string& line) {
    int sep = line.find('|');
    string name = line.substr(0, sep);

    int sep2 = line.find('|', sep + 1);
    //cout << "8: "<< line.substr(sep + 1, sep2 - sep - 1)<<endl;
    int id = stoi(line.substr(sep + 1, sep2 - sep - 1));

    sep = sep2;
    sep2 = line.find('|', sep + 1);
    string date = line.substr(sep + 1, sep2 - sep - 1);
    int s1 = date.find(',');
    int s2 = date.find(',', s1 + 1);
    //cout << "9: "<< date<<endl;
    int day = stoi(date.substr(0, s1));
    int month = stoi(date.substr(s1 + 1, s2 - s1 - 1));
    int year = stoi(date.substr(s2 + 1));

    sep = sep2;
    sep2 = line.find('|', sep + 1);
    //cout << "10: "<< line.substr(sep + 1, sep2 - sep - 1)<<endl;
    int OwnerID = stoi(line.substr(sep + 1, sep2 - sep - 1));
    sep = sep2;
    sep2 = line.find('|', sep + 1);
    // cout << "11: "<< line.substr(sep + 1, sep2 - sep - 1)<<endl;
    int activityNum = stoi(line.substr(sep + 1, sep2 - sep - 1));

    sep = sep2;
    sep2 = line.find('|', sep + 1);
    string activityValue = line.substr(sep + 1, sep2 - sep - 1);


    sep = sep2;
    sep2 = line.find('|', sep + 1);
    string text = line.substr(sep + 1, sep2 - sep - 1);


    sep = sep2;
    sep2 = line.find('|', sep + 1);
    string LikedBy = line.substr(sep + 1, sep2 - sep - 1);
    int size = 0;
    for (int i = 0; LikedBy[i] != '\0'; i++) {
        if (LikedBy[i] == ',')
            size++;
    }
    size++;
    int* arr = new int[size];
    int index = 0;
    string temp = "";
    for (int i = 0; i < LikedBy.size(); i++) {
        if (LikedBy[i] == ' ')i++;
        while (LikedBy[i] != ',' && i < LikedBy.size()) {
            temp += LikedBy[i];
            i++;
        }
        //cout << "12: "<< temp<<endl;
        arr[index++] = stoi(temp);
        temp = "";

    }


    sep = sep2;
    sep2 = line.find('|', sep + 1);
    string comments = line.substr(sep + 1, sep2 - sep - 1);
    int commentCount = 0;
    int* commentsIDs = nullptr;
    if (comments != " ") {
        for (int i = 0; comments[i] != '\0'; i++) {
            if (comments[i] == ',')
                commentCount++;
        }
        commentCount++;
        commentsIDs = new int[commentCount];
        index = 0;
        temp = "";
        for (int i = 0; i < comments.size(); i++) {
            if (comments[i] == ' ')i++;
            while (comments[i] != ',' && i < comments.size()) {
                temp += comments[i];
                i++;
            };
            // cout << "13: "<< line.substr(sep + 1, sep2 - sep - 1)<<endl;
            commentsIDs[index++] = stoi(temp);
            temp = "";

        }
    }


    if (OwnerID > 90000) {

        for (int i = 0; i < page::getCount(); i++) {
            if (p[i]->getID() == OwnerID) {
                p[i]->addPost(id, day, month, year, activityNum, activityValue, text, arr, size, this, comments);
            }
        }
    }
    else {
        for (int i = 0; i < user::getCount(); i++) {
            if (u[i]->getID() == OwnerID) {
                u[i]->addPost(id, day, month, year, activityNum, activityValue, text, arr, size, this, comments);
            }
        }
    }
}
void SocialNetworkApp::addPage(int id,const string& name) {
    page::incrementCount();
    int num = page::getCount();
    page** temp = new page * [num];
    (temp[num - 1]) = new page(id, name);
    for (int i = num - 2; i >= 0; i--) {
        temp[i] = p[i];
    }
    delete[]p;
    p = temp;
}
void SocialNetworkApp::addUser(int id, const string &name) {
    user::incrementCount();
    int num = user::getCount();
    user** temp = new user * [num];
    (temp[num - 1]) = new user(id, name);
    for (int i = num - 2; i >= 0; i--) {
        temp[i] = u[i];
    }
    
    delete[]u;
    u = temp;
}
void SocialNetworkApp::setMemory(const string& line) {

    int sep = line.find('|');
    //cout << "16: "<<line.substr(0, sep)<<endl;
    int memID = stoi(line.substr(0, sep));

    int sep2 = line.find('|', sep + 1);
    //cout << "17: "<< line.substr(sep + 1, sep2 - sep - 1) <<endl;
    int pID = stoi(line.substr(sep + 1, sep2 - sep - 1));

    sep = sep2;
    sep2 = line.find('|', sep + 1);
    // cout << "18: "<< line.substr(sep + 1, sep2 - sep - 1) <<endl;
    int uID = stoi(line.substr(sep + 1, sep2 - sep - 1));

    sep = sep2;
    sep2 = line.find('|', sep + 1);
    string caption = line.substr(sep + 1, sep2 - sep - 1);

    sep = sep2;
    sep2 = line.find('|', sep + 1);
    string date = line.substr(sep + 1, sep2 - sep - 1);
    int s1 = date.find(',');
    int s2 = date.find(',', s1 + 1);
    //cout << "15: "<< date<<endl;
    int day = stoi(date.substr(0, s1));
    int month = stoi(date.substr(s1 + 1, s2 - s1 - 1));
    int year = stoi(date.substr(s2 + 1));


    sep = sep2;
    sep2 = line.find('|', sep + 1);
    string LikedBy = line.substr(sep + 1, sep2 - sep - 1);
    int size = 0;
    for (int i = 0; LikedBy[i] != '\0'; i++) {
        if (LikedBy[i] == ',')
            size++;
    }

    size++;
    int* arr = new int[size];
    int index = 0;
    string temp = "";
    if(LikedBy !=" ")
    for (int i = 0; i < LikedBy.size(); i++) {
        if (LikedBy[i] == ' ')i++;
        while (LikedBy[i] != ',' && i < LikedBy.size()) {
            temp += LikedBy[i];
            i++;
        }
        //cout << "16: "<< temp<<endl;
        arr[index++] = stoi(temp);
        temp = "";

    }
    else {
        arr=nullptr;
        size=0;
    }
    sep = sep2;
    sep2 = line.find('|', sep + 1);
    string comments = line.substr(sep + 1, sep2 - sep - 1);


    user* ptr = getUserPtr( uID);
    ptr->addMemory(memID, caption, day, month, year, pID, this, arr, size, comments);

}
void post::setLikedby(user** u, page** p, int* arr, int c) {
    likeCount = c;
    likedBy = new user * [likeCount];
    for (int i = 0; i < likeCount; i++) {
        for (int j = 0; j < user::getCount(); j++) {
            if (u[j]->getID() == arr[i]) {
                likedBy[i] = u[j];
            }
        }
    }
}
void post::addComment(int id, int userID, string text, int d, int m, int y,SocialNetworkApp* ptr) {
    page* pageptr=nullptr;
    user* userptr=nullptr;
    user** u=ptr->u;
    page**p=ptr->p;
    if (userID > 90000) {

        for (int i = 0; i < page::getCount(); i++) {
            if (p[i]->getID() == userID) {
                pageptr = p[i];
                break;
            }
        }
    }

    else {

        for (int i = 0; i < user::getCount(); i++) {
            if (u[i]->getID() == userID) {
                userptr = u[i];
                break;
            }
        }
    }
    commentCount++;
    comment** temp = new comment * [commentCount];
    if(userptr!=nullptr)
    (temp[commentCount - 1]) = new comment(id, text, d, m, y, userptr);
    else   
    (temp[commentCount - 1]) = new comment(id, text, d, m, y, pageptr);
    for (int i = commentCount - 2; i >= 0; i--) {
        temp[i] = comments[i];
    }

    delete[]comments;
    comments = temp;

}
void post::addLike(user * current)
{
    likeCount++;
    user** temp = new user * [likeCount];
    temp[likeCount - 1] = current;
    for (int i = likeCount - 2; i >= 0; i--) {
        temp[i] = likedBy[i];
    }

    delete[]likedBy;
    likedBy = temp;
}
void page::addPost(int i, int d, int m, int y, int activityNum, string actValue, string text, int* likedBy, int lCount, SocialNetworkApp*ptr, string commentstr) {
    postCount++;
    post** temp = new post * [postCount];
    user** u=ptr->u;
    page**p=ptr->p;
    (temp[postCount - 1]) = new post(i, d, m, y, activityNum, actValue, text,this);
    temp[postCount - 1]->setLikedby(u, p, likedBy, lCount);
    ptr->readComments(commentstr, temp[postCount - 1]);
    for (int i = postCount - 2; i >= 0; i--) {
        temp[i] = posts[i];
    }

    delete[]posts;
    posts = temp;
}
void page::setLikedy(user** all, int* arr, int c) {
    likeCount = c;
    likedBy = new user * [likeCount];
    for (int i = 0; i < likeCount; i++) {
        for (int j = 0; j < user::getCount(); j++) {
            if (all[j]->getID() == arr[i]) {
                likedBy[i] = all[j];
            }
        }
    }
}
void user:: addMemory(int i, string t, int d, int m, int y, int pID, SocialNetworkApp* ptr, int* likedBy, int lCount, string commentstr) {
        postCount++;
        user** u=ptr->u;
        page**p=ptr->p;
        post** temp = new post * [postCount];
        post* postptr = nullptr;

        for (int i = 0; i < postCount - 1; i++) {
            if (posts[i]->getID() == pID) {
                postptr = posts[i];
            }
        }
        if (ptr==nullptr){
            cout << "\nPost not found in User's timeline for memory. "<<endl;
            return;
        }
        temp[postCount - 1] = new memory(i, d, m, y, t, postptr, this);
        ptr->readComments(commentstr, temp[postCount - 1]);
        temp[postCount - 1]->setLikedby(u, p, likedBy, lCount);
        for (int i = postCount - 2; i >= 0; i--) {
            temp[i] = posts[i];
        }

        delete[]posts;
        posts = temp;
    }
void user:: addFriend(user* ptr){
        friendsCount++;
        user** temp = new user * [friendsCount];
        temp[friendsCount - 1] = ptr;
        for (int i = friendsCount - 2; i >= 0; i--) {
            temp[i] = friends[i];
        }
        delete[]friends;
        friends = temp;
    }
void user::addLikePage(page* ptr){
        pagesCount++;
        page** temp = new page * [pagesCount];
        temp[pagesCount - 1] = ptr;
        for (int i = pagesCount - 2; i >= 0; i--) {
            temp[i] = likedPages[i];
        }
        delete[]friends;
        likedPages = temp;
}
void user:: addPost(int i, int d, int m, int y, int activityNum, string actValue, string text, int* likedBy, int lCount, SocialNetworkApp* ptr, string commentstr) {
        postCount++;
        post** temp = new post * [postCount];
        user** u=ptr->u;
        page**p=ptr->p;
        temp[postCount - 1] = new post(i, d, m, y, activityNum, actValue, text, this);
        ptr->readComments(commentstr, temp[postCount - 1]);
        temp[postCount - 1]->setLikedby(u, p, likedBy, lCount);
        for (int i = postCount - 2; i >= 0; i--) {
            temp[i] = posts[i];
        }

        delete[]posts;
        posts = temp;
    }
void user:: setPages(page** all, int* arr, int c) {
        pagesCount = c;
        likedPages = new page * [pagesCount];
        for (int i = 0; i < pagesCount; i++) {
            for (int j = 0; j < page::getCount(); j++) {
                if (all[j]->getID() == arr[i]) {
                    likedPages[i] = all[j];
                }
            }
        }
    }
void user:: setfriends(user** all, int* arr, int c) {

        friendsCount = c;
        friends = new user * [friendsCount];
        for (int i = 0; i < friendsCount; i++) {
            for (int j = 0; j < count; j++) {
                if (all[j]->id == arr[i]) {
                    friends[i] = all[j];
                }
            }
        }
    }


//reading
int SocialNetworkApp::countEntries(const string& filename) {
    ifstream read(filename);
    string line;
    int count = 0;
    while (getline(read, line)) {
        count++;
    }
    return count;
} 
int SocialNetworkApp::getNextId(const string& filename){
    ifstream read(filename);
    string line;
    int num=countEntries(filename);
    for (int i=0; i<num;i++){
        getline(read,line);
    }
    int n= stoi(line.substr(0, line.find('|')));
    n++;
    read.close();
    return n;
    
}
int SocialNetworkApp::getNextID2(const string& filename){//for page or user 
    ifstream read(filename);
    string line;
    int num=countEntries(filename);
    for (int i=0; i<num;i++){
        getline(read,line);
    }
    int sep= line.find("||");
    int sep2= line.find("||",sep+2);
    int n= stoi(line.substr(sep+2,sep2-sep-1));
    n++;
    return n;
}
int SocialNetworkApp::readID(const string& filename, int num) {
    ifstream read(filename);
    string line;
    for (int i = 0; i < num; i++) {
        getline(read, line);
    }
    if (getline(read, line)) {
        int sep = line.find("||");
        int sep2 = line.find("||", sep + 2);
        //cout << "1: "<< line.substr(sep + 2, sep2 - sep - 2)<< endl;
        return stoi(line.substr(sep + 2, sep2 - sep - 2));
    }
    return -1;
}
string SocialNetworkApp::readName(const string& filename, int num) {
    ifstream read(filename);
    string line;
    for (int i = 0; i < num; i++) {
        getline(read, line);
    }
    if (getline(read, line)) {
        int sep = line.find("||");
        return line.substr(0, sep);

    }
    return "";
}
int* SocialNetworkApp::readCol3(const string& filename, int& size, int num) {
    ifstream read(filename);
    string line;
    int* arr=nullptr;
    for (int i = 0; i < num; i++) {
        getline(read, line);
    }
    if (getline(read, line)) {
        int sep = line.find("||");
        sep = line.find("||", sep + 2);
        int sep2 = line.find("||", sep + 2);
        string friends = line.substr(sep + 2, sep2 - sep - 2);

        
        if(friends!=" "){
        for (int i = 0; friends[i] != '\0'; i++) {
            if (friends[i] == ',')
                size++;
        }
        size++;
        arr = new int[size];
        int index = 0;
        string temp = "";
        for (int i = 0; i < friends.size(); i++) {
            if (friends[i] == ' ')i++;
            while (friends[i] != ',' && i < friends.size()) {
                temp += friends[i];
                i++;
            }
            //cout << "2: "<< temp <<endl;
            arr[index++] = stoi(temp);
            temp = "";

        }
    }
    else {
        arr=nullptr;
        size=0;
    }
        
    }
    return arr;
}
int* SocialNetworkApp::readCol4(const string& filename, int& size, int num) {
    ifstream read(filename);
    string line;
    int*arr=nullptr;
    for (int i = 0; i < num; i++) {
        getline(read, line);
    }
    if (getline(read, line)) {
        int sep = line.find("||");
        sep = line.find("||", sep + 2);
        sep = line.find("||", sep + 2);
        int sep2 = line.find("||", sep + 2);
        string pages = line.substr(sep + 2, sep2 - sep - 2);

        
    if (pages!=" "&&pages!=""){
        for (int i = 0; pages[i] != '\0'; i++) {
            if (pages[i] == ',')
                size++;
        }
        size++;
        arr = new int[size];
        int index = 0;
        string temp = "";
        for (int i = 0; i < pages.size(); i++) {
            if (pages[i] == ' ')i++;
            while (pages[i] != ',' && i < pages.size()) {
                temp += pages[i];
                i++;
            }
            //cout << "3: "<< temp <<endl;
            arr[index++] = stoi(temp);
            temp = "";
        }
    }
        else {
            size=0;
            arr=nullptr;
        }
        

        
    }
    return arr;
}

//loading
void SocialNetworkApp::loadData() {

    //adding users
    string filename1 = "user.txt";
    int temp = countEntries(filename1);
    for (int i = 0; i < temp; i++) {
        addUser(readID(filename1, i), readName(filename1, i));
    }

    //adding pages
    string filename2 = "page.txt";
    int temp1 = countEntries(filename2);
    for (int i = 0; i < temp1; i++) {
        addPage(readID(filename2, i), readName(filename2, i));
    }

    // set friends
    int t = 0;
    int* friendIDs;
    for (int i = 0; i < temp; i++) {
        friendIDs = readCol3(filename1, t, i);
        u[i]->setfriends(u, friendIDs, t);
        t = 0;
    }

    //set likedPages
    int c = 0;
    int* pagesIDs;
    for (int i = 0; i < temp; i++) {
        pagesIDs = readCol4(filename1, c, i);
        u[i]->setPages(p, pagesIDs, c);
        c = 0;
    }

    //adding posts
    string filename3 = "post.txt";
    int temp2 = countEntries(filename3);
    ifstream read(filename3);
    for (int i = 0; i < temp2; i++) {
        string line;
        getline(read, line);
        addPost(line);
    }

    //adding memories
    string filename4 = "memories.txt";
    int temp3 = countEntries(filename4);
    ifstream read2(filename4);
    for (int i = 0; i < temp3; i++) {
        string line;
        getline(read2, line);
        setMemory(line);
    }


    //adding likedby(page)
    int l = 0;
    int* userIDs;
    for (int i = 0; i < temp1; i++) {
        userIDs = readCol3(filename2, l, i);
        p[i]->setLikedy(u, userIDs, l);
        l = 0;
    }

    for(int i=0; i<user::getCount();i++){
        u[i]->sortPosts();
    }
    for (int i=0; i<page::getCount();i++){
        p[i]->sortPosts();
    }

}

//Writing funcs      ----
void SocialNetworkApp::writeNewMem(int mID,int pID,int uID, const string& caption,const date& d){
    string filename="memories.txt";
    ofstream write(filename, ios::app);
    write << endl<< mID << "|" << pID << "|" << uID << "|" << caption << "|" << d.getDay() << "," << d.getMonth() << "," << d.getYear()<<"| "<<"| |" ;
}
void SocialNetworkApp::writeNewCom(int pID,const string& arr,int uID,int d, int m, int y){
    ofstream write("comment.txt",ios::app); 
    int id =getNextId("comment.txt");
    write << "\n" << id <<"|" <<d<<","<<m<<","<<y<<"|"<<uID<<"|"<<arr<<"|";
    write.close();
    
    string content="";
    string line;
    string IDstr=to_string(id);
    int sep,sep2;
    string filename="";
    if(pID>30000){
        filename="memories.txt";
            ifstream read(filename);
    while(!(read.eof())){
        getline(read,line);
        sep=line.find('|');
        if (stoi(line.substr(0,sep))==pID){
            sep=line.rfind('|');
            sep2=line.rfind('|',sep-1);
            if(line[sep-2]=='|'&&line[sep-1]==' '){
                line.insert(sep,IDstr);
            }
            else{
            line.insert(sep,","+IDstr);
            }
        }
        content+=line+'\n';
    }   
        content.pop_back();
        read.close();
        write.open(filename);
        write <<content;
        write.close();
    }
    else {
    filename="post.txt";
    ifstream read(filename);
    while(!(read.eof())){
        getline(read,line);
        sep=line.find('|');
        if (stoi(line.substr(sep+1,line.find('|',sep+1)-sep-1))==pID){
            sep=line.rfind('|');
            sep2=line.rfind('|',sep-1);
            if(line[sep-2]=='|'&&line[sep-1]==' '){
                line.insert(sep,IDstr);
            }
            else{
            line.insert(sep,","+IDstr);
            }
        }
        content+=line+'\n';
    }   
        content.pop_back();
        read.close();
        write.open(filename);
        write <<content;
        write.close();
}
}
void SocialNetworkApp::writeLikePost(int id,int pID){
    string content="";
    string line;
    string IDstr=to_string(id);
    int sep,sep2;
    string filename="";
    if(pID>30000){
        filename="memories.txt";
        ifstream read(filename);
    
    while(!(read.eof())){
        getline(read,line);
        sep=line.find('|');
        if (stoi(line.substr(0,sep))==pID){
            sep=line.rfind('|');
            sep2=line.rfind('|',sep-1);
            if(line[sep2-2]=='|'&&line[sep2-1]==' '){
                line.insert(sep2,IDstr);
            }
            else{
                line.insert(sep2,","+IDstr);
            }
        }
        content+=line+'\n';
    }   
        content.pop_back();
        read.close();
        ofstream write(filename);
        write <<content;
        write.close();
    }
    else {
        filename="post.txt";
    
    ifstream read(filename);
    
    while(!(read.eof())){
        getline(read,line);
        sep=line.find('|');
        if (stoi(line.substr(sep+1,line.find('|',sep+1)-sep-1))==pID){
            sep=line.rfind('|');
            sep2=line.rfind('|',sep-1);
            if(line[sep2-2]=='|'&&line[sep2-1]==' '){
                line.insert(sep2,IDstr);
            }
            else{
                line.insert(sep2,","+IDstr);
            }
        }
        content+=line+'\n';
    }   
        content.pop_back();
        read.close();
        ofstream write(filename);
        write <<content;
        write.close();
}
}
void SocialNetworkApp::writeNewUser(const string& name, int id){
    ofstream write("user.txt", ios::app);
    write << endl << name << "||" << id << "|| || ";
    write.close();
}
void SocialNetworkApp::writeNewFriend(int id,int fID){
    string content="";
    string line;
    string IDstr=to_string(fID);
    int sep,sep2;
    ifstream read("user.txt");
    
    while(!(read.eof())){
        getline(read,line);
        sep=line.find("||");
        int sep2 = line.find("||", sep + 2);
        if (stoi(line.substr(sep+2,sep2-sep-1))==id){
            sep=line.rfind("||");
            if(line[sep-2]=='|' &&line[sep-1]==' '){
                line.insert(sep,IDstr);
            }
            else{
                line.insert(sep,","+IDstr);
            }
        }
        content+=line+'\n';
    }  
        content.pop_back();
        read.close();
        ofstream write("user.txt");
        write <<content;
        write.close();
}
void SocialNetworkApp::writeLikePage(int id, int pageID){
    string content="";
    string line;
    string IDstr=to_string(pageID);
    int sep,sep2;
    ifstream read("user.txt");
    
    while(!(read.eof())){
        getline(read,line);
        sep=line.find("||");
        int sep2 = line.find("||", sep + 2);
        if (stoi(line.substr(sep+2,sep2-sep-1))==id){
            if(line[line.length()-1]==' ' &&line[line.length()-2]=='|'){
                line.pop_back();
                line+=IDstr;
            }
            else{
                line+=","+IDstr;
            }
        }
        content+=line+'\n';
    }  
        content.pop_back();
        read.close();
        ofstream write("user.txt");
        write <<content;
        write.close();
}


//helper funcs
bool post:: isLiked(int uid){
        for (int i=0; i<likeCount;i++){
            if (uid==likedBy[i]->getID()){
                return true;
            }
        }
        return false;
    }
void page:: sortPosts() {
        //bubble sort
        for (int i = 0; i < postCount - 1; i++)
        {
            for (int j = 0; j < postCount - i - 1; j++)
            {
                const date& date1 = posts[j]->getDate();
                const date& date2 = posts[j + 1]->getDate();

                // Compare years first
                if (date1.getYear() < date2.getYear() ||
                 (date1.getYear() == date2.getYear() &&
                    date1.getMonth() < date2.getMonth()) ||
                // If same year and month compare days
                (date1.getYear() == date2.getYear() &&
                    date1.getMonth() == date2.getMonth() &&
                    date1.getDay() < date2.getDay()))
            
                    // Swap
                post* temp = posts[j];
             if (date1.getYear() < date2.getYear() ||
                // If same year compare months
                (date1.getYear() == date2.getYear() &&
                 date1.getMonth() < date2.getMonth()) ||
                // If same year and month compare days
                (date1.getYear() == date2.getYear() &&
                 date1.getMonth() == date2.getMonth() &&
                 date1.getDay() < date2.getDay()))
            {
                // Swap
                post* temp = posts[j];
                posts[j] = posts[j + 1];
                posts[j + 1] = temp;
            }
        }
    }
}
void user:: sortPosts() {
        //bubble sort
        for (int i = 0; i < postCount - 1; i++)
        {
            for (int j = 0; j < postCount - i - 1; j++)
            {
                const date& date1 = posts[j]->getDate();
                const date& date2 = posts[j + 1]->getDate();
                // Compare years first
                if (date1.getYear() < date2.getYear() ||
                    // If same year compare months
                    (date1.getYear() == date2.getYear() &&
                        date1.getMonth() < date2.getMonth()) ||
                    // If same year and month compare days
                    (date1.getYear() == date2.getYear() &&
                        date1.getMonth() == date2.getMonth() &&
                        date1.getDay() < date2.getDay()))
                {
                    // Swap
                    post* temp = posts[j];
                    posts[j] = posts[j + 1];
                    posts[j + 1] = temp;
                }
            }
        }
    }
bool user:: istimeline(post*ptr){
        for (int i=0; i<postCount;i++){
            if (ptr->getID()==posts[i]->getID())
                return true;
        }
        return false;
    }
bool user:: isFriend(user *ptr){
    for (int i=0; i<friendsCount;i++){
        if (ptr->getID()==friends[i]->getID()){
            return true;
        }
    }
    return false;
}
bool user:: isLiked(page* ptr){
    for (int i=0; i<pagesCount;i++){
        if (ptr->getID()==likedPages[i]->getID()){
            return true;
        }
    }
    return false;
}

//view funcs
void page::viewPosts(const date& currentDate) const { //check for past 24 hours pending
    for (int i = 0; i < postCount; i++) {
        if (posts[i]->getDate().isWithin24Hours(currentDate)) {
            posts[i]->view();
            cout << endl;
        }
    }
}
void post::viewLikedByList(user** allUsers, page** allPages)const
{
    cout << "-----------------------------------------------------------" << endl;
    cout << "Post Liked By : " << endl<<endl;
    for (int i = 0; i < likeCount; i++)
    {
        int likerId = likedBy[i]->getID();
        if (likerId > 90000)//page
        {
             for (int j = 0; j < page::getCount(); j++)
            {
                if (allPages[j]->getID() == likerId)
                {
                    cout << "P-" << likerId << "-" << allPages[j]->getName() << endl;
                    break;
                }
            }
        }
        else//user
        {
            for (int j = 0; j < user::getCount(); j++)
            {
                if (allUsers[j]->getID() == likerId)
                {
                    cout << "U-" << likerId << "-" << allUsers[j]->getName() << endl;
                    break;
                }
            }
        }
    }
}
void user::viewAllUsers(user** allUsers,int &index) {
    cout << "-----------------------------------------------------------" << endl;
    cout << "All Users List:" << endl<<endl;
    cout << "-----------------------------------------------------------" << endl;
    
    int counter = 1;
    for (int i = 0; i < user::getCount(); i++) {
        if (allUsers[i] != nullptr) 
        {
            if (allUsers[i] == this) 
            {   index=counter;
                cout << counter << ". [Current User - " << allUsers[i]->getName() << "]" << endl;
                counter++;
                continue;
            }
            cout << counter << ". " << allUsers[i]->getName() << endl;
            counter++;
        }
    }
    cout << "-----------------------------------------------------------" << endl;
}
void user::viewLikedPagesList(page** allPages) const{
    cout << "-----------------------------------------------------------" << endl;
    cout << name << "'s Liked Pages (" << pagesCount << " pages):" << endl<<endl;
    
    for (int i = 0; i < pagesCount; i++) {
        if (likedPages[i] != nullptr) {  // Check if page pointer is valid
            int pageId = likedPages[i]->getID();
            bool found = false;
            
            // Search through all pages to find matching ID
            for (int j = 0; j < page::getCount(); j++) {
                if (allPages[j]->getID() == pageId) {
                    cout << "PG-" << pageId << "-" << allPages[j]->getName() << endl;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                cout << "PG-" << pageId << "-[Unknown Page]" << endl;
            }
        }
    }
    cout << "------------------------------------------------------------" << endl;
}
void user::viewFriendsList(user** allUsers) const 
{
    cout << "-----------------------------------------------------------\n";
    cout << name << "'s Friends List (" << friendsCount << " friends):"<<endl<<endl;
    
    for (int i = 0; i < friendsCount; i++) {
        if (friends[i]) {
            cout << "U- " << friends[i]->getName() << "\n";
        }
    }
    cout << "-----------------------------------------------------------\n";
}
void SocialNetworkApp::viewPagesList() {
    cout << "-----------------------------------------------------------" << endl;
    cout << "All Pages List:" << endl<<endl;
    cout << "-----------------------------------------------------------" << endl;
    
    for (int i = 0; i < page::getCount(); i++) {
        if (p[i] != nullptr) {
            cout << "PG-" << p[i]->getID() << " - " << p[i]->getName() << endl;
        }
    }
    
    cout << "-----------------------------------------------------------" << endl;
}
void page::viewPage() const
{
    cout << "------------------------------------------------------------------" << endl;
    cout << "\t\t\t\t--------"<<name << "--------" << endl<<endl;
    //posts
    for (int i = 0; i < postCount; i++)
    {
        posts[i]->view();
        cout << endl;
    }
    cout << "------------------------------------------------------------------" << endl;
}
void post::view()
{
    if(isMemory()){
        displayMemory();
    }
    else
    { if (ownerUser!=nullptr) {
        cout <<"-----"<< ownerUser->getName();
    }
    else if (ownerPage!=nullptr) {
        cout <<"-----"<< ownerPage->getName(); 
    }
    if (act)
    {
       
        act->printActivity();
        cout <<endl<<"\t\t\""<< arr<<"\""<< " ...";//////////////////////////////
    }
    else
    {
        cout << " shared \n\t\t" << arr << "\"";
    }
    cout << "...............(";
    d.print();
    cout << ")\n\n";

    //comments
    for (int i = 0; i < commentCount; i++)
    {
        cout << "\t\t\t";
        if (comments[i]->getUser()!=nullptr) {
            cout << comments[i]->getUser()->getName();
        }
        else if (comments[i]->getPage()!=nullptr) {
            cout << comments[i]->getPage()->getName();
        }
        cout << " wrote: \"" << comments[i]->getArr() << "\"" << endl;
    }
    cout <<"\t\t\t\t\t\t\t\t\t\t\t\t\t(P-"<<this->id<<")";
    cout << endl<<endl;
}
}
void memory::displayMemory() const
{
    int yearsAgo = date::getCurrentYear() - postDate.getYear();
    if(yearsAgo==0)
    {
        cout<<"This years memory :"<<endl;
    }
    else
    {
        cout << yearsAgo << (yearsAgo == 1 ? " Year Ago" : " Years Ago") << endl;
    }

    // Display who shared the memory (user or page)
    if (ownerUser!=nullptr) {
        cout <<"-----"<< ownerUser->getName();
    }
    else if (ownerPage!=nullptr) {
        cout <<"-----" << ownerPage->getName();
    }

    cout << " shared a memory (";
    postDate.print();
    cout << ")" << endl;

     cout << "--- " << memCaption << endl;
    cout << "\t\t\"" << getText() << "\"" << endl;

    cout << "Memory created on:.... (";
    d.print();
    cout << ")" << endl;
    
    for (int j = 0; j < getCommentCount(); j++) {
        comment* c = getComment(j);
        cout << "\t\t\t";

        if (c->getUser()!=nullptr) {
            cout << c->getUser()->getName();
        }
        else if (c->getPage()!=nullptr) {
            cout << c->getPage()->getName();
        }

        cout << " wrote: \"" << c->getArr() << "\"" << endl;
    }
    cout <<"\t\t\t\t\t\t\t\t\t\t\t\t\t(P-"<<this->id<<")";
    cout << endl<<endl;
}
void user::viewMemories()const
    {
        cout << "------------------------------------------------------------------" << endl;
        cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago." << endl;
        bool foundMemory = false;
        for (int i = 0; i < postCount; i++) {
            if (posts[i]->isMemory()) {
                posts[i]->displayMemory();
                foundMemory = true;
            }
        }

        if (!foundMemory) {
            cout << "No memories found." << endl;
        }
        cout << "------------------------------------------------------------------" << endl;
    }
void user::viewTimeline()const
    {
        cout << "----------------------------------------------------------------------" << endl;
        cout <<"\t\t" <<name << "- Time Line" << endl<<endl;
        //post
        for (int i = 0; i < postCount; i++)
        {
             if (posts[i]->isMemory())
             {
                posts[i]->displayMemory();
             }
            if (posts[i]->getActivity()!=nullptr)
            { 
                cout<<name<<" ";
                posts[i]->getActivity()->printActivity();
                cout<<endl<< posts[i]->getText();

            }
            else
            {
                cout <<"-----"<<name<< " shared \"" << posts[i]->getText() << "\"";
            }
            // Post date
            cout << " .............(";
            posts[i]->getDate().print();
            cout << ")" << endl;
            // Display comments
            for (int j = 0; j < posts[i]->getCommentCount(); j++)
            {
                comment* c = posts[i]->getComment(j);
                 cout << "\t";
            if (c->getUser()!=nullptr) {
                cout << c->getUser()->getName();
            }
            else if (c->getPage()!=nullptr) {
                cout << c->getPage()->getName();
            }
            cout << " wrote: \"" << c->getArr() << "\"" << endl;
            }
            cout <<"\t\t\t\t\t\t\t\t\t\t\t(P-"<<posts[i]->getID()<<")";
            cout << endl;
        }
        cout << "------------------------------------------------------------------" << endl;
    }
void user::viewHome(const date& currentDate)
    {
        date previousDate = currentDate.getPreviousDate();

        cout << "------------------------------------------------------------------" << endl;
        cout << name << " - Home Page" << endl;
        cout << "//This is displaying posts of " << previousDate.getFormattedDate()
            << " and " << currentDate.getFormattedDate()
            << " (" << currentDate.getFormattedDate() << " is currentDate)" << endl;
        cout << "------------------------------------------------------------------" << endl;

        for (int i = 0; i < pagesCount; i++) {
            if (likedPages[i] != nullptr) {
                likedPages[i]->viewPosts(currentDate);
            }
        }
        for (int i = 0; i < friendsCount; i++) {
            if (friends[i] != nullptr) {
                friends[i]->viewUserPosts(currentDate);
            }
        }
    }
void user::viewUserPosts(const date& currentDate) const {
        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getDate().isWithin24Hours(currentDate)) {
                posts[i]->view();
                cout << endl;
            }
        }
    }


//user interface1
user* SocialNetworkApp::setUser(){
    cout << "--Enter your id: U-";
    int id;
    cin >>id;
    user *currentUser=getUserPtr( id);
    while (currentUser == nullptr) {
        cout << "\n--No user with this ID exists, Enter again: ";
        cin >> id;
        currentUser = getUserPtr( id);
    }
    cout << endl <<endl <<"--"<<currentUser->getName() << " set as current user\n\n";
    cout <<"_________________________________________\n\n";
    return currentUser;
}
void SocialNetworkApp::commentOnPost(user* current){
     cout << "----Enter post ID to comment on(Enter 0 to cancel ): P-";
            int postID; 
            cin >> postID;
            if (postID!=0){
            post* postptr=findPostU(postID);
            while(postptr==nullptr){
                cout <<"Post not found. Enter ID again: P-";
                cin.ignore();
                cin.clear();
                cin >> postID;
                postptr=findPostU(postID);
            }
            cout << "----Enter your comment: ";
            string commentText;
            cin.ignore();
            getline(cin, commentText);
            
            if (postptr->getCommCount()<10){
                postptr->addComment(getNextId("comment.txt"),current->getID(),commentText,systemDate.getDay(),systemDate.getMonth(),systemDate.getYear(),this);
                writeNewCom(postID,commentText,current->getID(),systemDate.getDay(),systemDate.getMonth(),systemDate.getYear());
                cout << "----Comment added successfully.\n";
            }
            else{
                cout << "----Comment limit reached for this post.\n";
            }
        }
}
void SocialNetworkApp::viewLikes(user* current){
    cout << "----Enter post ID to view likes(Enter 0 to cancel) : P-";
            int postID;
            cin >> postID;
            if(postID!=0){
            post* postptr=findPostU(postID);
            while(postptr==nullptr){
                cout <<"Post not found. Enter ID again: P-";
                cin >> postID;
                postptr=findPostU(postID);
            }
            postptr->viewLikedByList(u,p);
        }
}
void SocialNetworkApp::LikePost(user* current){ 
            cout << "----Enter post ID to like(Enter 0 to cancel ): P-";
            int postID;
            cin >> postID;
            if(postID!=0){
            post* postptr=findPostU(postID);
            while(postptr==nullptr){
                cout <<"Post not found. Enter ID again: P-";
                cin >> postID;
                postptr=findPostU(postID);
            }
                if(!postptr->isLiked(current->getID())){
                    writeLikePost(current->getID(),postID);
                    postptr->addLike(current);
                    cout << "Like added successfully.\n";
                }
                else   
                    cout << "Post is already liked. \n";
            }
            
}
void SocialNetworkApp::viewPostbyID(user* current){
        cout << "----Enter post ID to view (Enter 0 to cancel ): P-";
        int postID;
        cin >> postID;
        if(postID!=0){
        post* postptr=findPostU(postID);
        while(postptr==nullptr){
                cout <<"Post not found. Enter ID again: P-";
                cin >> postID;
                postptr=findPostU(postID);
            }
            postptr->view();
            postptr->viewLikedByList(u,p);
        while(true){
        cout << "\n\n\n---Commands: \n\n1: Like this Post\n2: Comment on this Post\n3: Back to Main Menu\n";
        cout <<"\n________________________________________\n\n---Enter your choice: ";
        int choice;
        cin >> choice;
        while (choice < 1 || choice > 3) {
            cout << "---Invalid Input, Enter again: ";
            cin >> choice;
        }
        if (choice==1){
            if(!postptr->isLiked(current->getID())){
                    writeLikePost(current->getID(),postID);
                    postptr->addLike(current);
                    cout << "Like added successfully.\n";
                }
                else   
                    cout << "Post is already liked. \n";
            
        }
        else if (choice==2){
            cout << "----Enter your comment: ";
            string commentText;
            cin.ignore();
            getline(cin, commentText);
            
            if (postptr->getCommCount()<10){
                postptr->addComment(getNextId("comment.txt"),current->getID(),commentText,systemDate.getDay(),systemDate.getMonth(),systemDate.getYear(),this);
                writeNewCom(postID,commentText,current->getID(),systemDate.getDay(),systemDate.getMonth(),systemDate.getYear());
                cout << "----Comment added successfully.\n";
            }
            else{
                cout << "----Comment limit reached for this post.\n";
            }
        }
        else if (choice==3){
            cout << "\n\n----Back to Main Menu\n";
            return;
        }
    }

        }
}
void SocialNetworkApp::addNewFriend(user*current){
    int currentIndex;
        current->viewAllUsers(u,currentIndex);
        cout << "---Enter choice to add(Enter 0 to cancel ): ";
        int friendChoice;
        cin >> friendChoice;
        while(friendChoice>user::getCount()||friendChoice<0||friendChoice==currentIndex){
            cout << "Invalid choice, Enter Again: ";
            cin >>friendChoice;
        }

        if(friendChoice!=0){
        user* friendptr=u[friendChoice-1];
        if(current->isFriend(friendptr)){
            cout << "User is already your friend.\n";
        }
        else
        {
        current->addFriend(friendptr);
        writeNewFriend(current->getID(),friendptr->getID());
        cout << "----"<< friendptr->getName()<<" added as friend added successfully.\n";
        }
    }
}
void SocialNetworkApp::home (user* current){
        current->viewHome(systemDate);
        while(true){
        cout << "\n\n\n---Commands: \n\n1: Like Post\n2: Comment on Post\n3: View Likes\n4: Back to Main Menu\n";
        cout <<"\n________________________________________\n\n---Enter your choice: ";
        int choice;
        cin >> choice;
        while (choice < 1 || choice > 4) {
            cout << "---Invalid Input, Enter again: ";
            cin >> choice;
        }
        if (choice==1){
            LikePost(current);
        }
        else if (choice==2){
            commentOnPost(current);
        }
        else if (choice==3){
            viewLikes(current);
        }
        else if (choice==4){
            cout << "\n\n----Back to Main Menu\n";
            return;
        }
    }
}
void SocialNetworkApp::Timeline(user* current){
    current->viewTimeline();
    while(true){
        cout << "\n\n\n---Commands: \n\n1: Like Post\n2: Share memory\n3: Comment on Post\n4: View Likes\n5: Back to Main Menu\n";
        cout <<"\n________________________________________\n\n---Enter your choice: ";
        int choice;
        cin >> choice;
        while (choice < 1 || choice > 5) {
            cout << "---Invalid Input, Enter again: ";
            cin >> choice;
        }
        if(choice==1){
            LikePost(current);
        }
        else if(choice==2){
            cout << "----Enter post ID to share memory(Enter 0 to cancel ): P-";
            int postID;
            cin >> postID;
            if(postID!=0){
            post* postptr=findPostU(postID);
            while(postptr==nullptr){
                cout <<"Post not found. Enter ID again: P-\n";
                cin >> postID;
                postptr=findPostU(postID);
            }
            if(current->istimeline(postptr)){ 
            cout << "----Enter your memory caption: ";
            string caption;
            cin.ignore();
            getline(cin, caption);
            current->addMemory(getNextId("memories.txt"),caption,systemDate.getDay(),systemDate.getMonth(),systemDate.getYear(),postID,this,nullptr,0," ");
            writeNewMem(getNextId("memories.txt"),postID,current->getID(),caption,systemDate);
            cout << "----Memory shared successfully.\n";
            current->sortPosts();
        }
        else 
            cout << "Cannot Share, Post is not in your timeline.\n\n";
        }
    }
        else if (choice==3){
            commentOnPost(current);
            
        }
        else if (choice==4){
            viewLikes(current);
        }
        else if (choice==5){
            cout << "\n\n----Back to Main Menu\n";
            return;
        }
    }
}
void SocialNetworkApp::pageview(user* current,page* pageptr){
    pageptr->viewPage();
        while(true){
        cout << "\n\n\n---Commands: \n\n1: Like Post\n2: Comment on Post\n3: View Likes on post\n4: Like this page\n5: Back to Main Menu\n";
        cout <<"\n________________________________________\n---Enter your choice: ";
        int choice;
        cin >> choice;
        while (choice < 1 || choice > 5) {
            cout << "---Invalid Input, Enter again: ";
            cin >> choice;
        }
        if (choice==1){
            LikePost(current);
        }
        else if (choice==2){
            commentOnPost(current);
        }
        else if (choice==3){
            viewLikes(current);
        }
        else if (choice==4){
            if (current->isLiked(pageptr)){
                cout << "Page is already liked.\n";
            }
            else {
                current->addLikePage(pageptr);
                writeLikePage(current->getID(),pageptr->getID());
                cout << "Liked "<< pageptr->getName()<< " successfully.";
            }
        }
        else if (choice==5){
            cout << "\n\n----Back to Main Menu\n";
            return;
        }
    }
}
void SocialNetworkApp::MainMenu(user *current) {
    while(true){
    cout << "--Commands: \n\n1: View Home\n2: View Timeline \n3: View Page \n4: View Friend list\n5: View Liked Pages\n6: Add friend\n7: View post by entering ID \n8: View Memories \n9: Logout\n";
    cout <<"________________________________________\n";
    int n;
    cin.ignore();
    cin.clear();
    cout << "--Enter your choice: ";
    cin >>n;
    while(n<1||n>9){
        cout << "--Ivalid Input, Enter again: ";
        cin>>n;
    }
    if (n==1){
        cout << "---Viewing Home...\n";
        cout << "------------------------------------------------------------------\n\n" ;
        home(current);
    }    
    else if(n==2){
        cout << "---Viewing Timeline...\n";
        cout << "------------------------------------------------------------------\n\n" ;
        Timeline(current);
    }
    else if (n==3){
        cout << "---Enter page ID to view (0 to view all pages) : PG-";
        int pageID;
        cin >> pageID;
        if (pageID==0){
                viewPagesList();
                cout << "---Enter page ID to view (Enter 0 to cancel): PG-";
                cin >> pageID;
            }
        if (pageID!=0){
        page* pageptr=getPagePtr(pageID);
        while (pageptr==nullptr){
            cout << "---Page not found. Enter again: PG-";
            cin>>pageID;
            pageptr=getPagePtr(pageID);
        }
        pageview(current,pageptr);
        }
    }
        else if (n==4){
        cout << "---Viewing Friends...\n";
        cout << "------------------------------------------------------------------\n\n" ;
        current->viewFriendsList(u);
    }
    else if (n==5){
        cout << "---Viewing Liked Pages...\n";
        cout << "------------------------------------------------------------------\n\n" ;
        current->viewLikedPagesList(p);
    }
    else if (n==6){
        addNewFriend(current);
}
    else if(n==7){
        viewPostbyID(current);
    }
    else if (n==8){
        current->viewMemories();
    }
    else if(n==9){
        cout << "\n\n---Logging out...\n\n";
        return;
        }
    }
}
void SocialNetworkApp:: run() {
    cout << "\t\t\t\t\tWelcome!\n\n\n";
    while(true){
    cout << "-Commands: \n\n1: Login\n2: Add new user\n3: Exit\n";
    cout << "________________________________________\n\n-Enter you choice: ";
    int n;
    cin >>n;
    while(n<1||n>3){
        cout << "-Ivalid Input, Enter again: ";
        cin>>n;
    }
    user* currentUser = nullptr;
    if (n==1){
        cout << "Setting User....\n\n";
        currentUser=setUser();
        MainMenu(currentUser);
    }
    else if (n==2){
        cout << "Enter Your name to setup new user: ";
        string name;
        cin.ignore();
        cin.clear();
        getline(cin,name);
        int id = getNextID2("user.txt");
        cout << "Your ID is: U-" << id << endl;
        cout << "Remember this ID, it cannot be recovered.\n\n";
        addUser(id,name);
        writeNewUser(name,id);
        cout << "User added successfully.\n\n";
        currentUser=setUser();
        MainMenu(currentUser);
    }

    else if(n==3){
        cout << "-Exiting...\n";
        return;
    }
}

}


int main() {
    SocialNetworkApp app;
    app.loadData();
    app.run();
    return 0;
}