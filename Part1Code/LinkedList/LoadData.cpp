// #include "LoadData.h"
// #include <iostream>
// #include <fstream>
// #include <sstream>

// using namespace std;

// // Constructors
// TransactionNode::TransactionNode(const Transaction& t) : data(t), next(nullptr) {}
// ReviewNode::ReviewNode(const Review& r) : data(r), next(nullptr) {}

// // CSV file to linked list 
// TransactionNode* loadTransactions(const string& filename) {
//     ifstream file(filename);
//     if (!file) {
//         cerr << "COuldnt open transaction file:";
//         return nullptr;
//     }

//     string line;
//     getline(file, line); // skip header 

//     TransactionNode* head = nullptr;
//     TransactionNode* tail = nullptr;

//     while (getline(file, line)) {
//         stringstream ss(line);
//         string CusID, Prod, Cat, PriceStr, Date, PayMethod;

//         getline(ss, CusID, ',');
//         getline(ss, Prod, ',');
//         getline(ss, Cat, ',');
//         getline(ss, PriceStr, ',');
//         getline(ss, Date, ',');
//         getline(ss, PayMethod);

//         if (PriceStr == "") PriceStr = "0";

//         Transaction t;
//         t.customerID = CusID;
//         t.product = Prod;
//         t.category = Cat;
//         t.price = atof(PriceStr.c_str());
//         t.date = Date;
//         t.paymentMethod = PayMethod;

//         TransactionNode* node = new TransactionNode(t);

//         if (!head) {
//             head = tail = node;
//         } else {
//             tail->next = node;
//             tail = node;
//         }

//         //cout << "[Transaction Added] " << t.customerID << " - " << t.product << endl;
//     }

//     return head;
// }

// // Same for reviews
// ReviewNode* loadReviews(const string& filename) {
//     ifstream file(filename);
//     if (!file) {
//         cerr << "Could not open review file";
//         return nullptr;
//     }

//     string line;
//     getline(file, line); // header skip

//     ReviewNode* head = nullptr;
//     ReviewNode* tail = nullptr;

//     while (getline(file, line)) {
//         stringstream ss(line);
//         string ProdID, CusID, RatingStr, RevText;

//         getline(ss, ProdID, ',');
//         getline(ss, CusID, ',');
//         getline(ss, RatingStr, ',');
//         getline(ss, RevText); 

//         Review r;
//         r.productID = ProdID;
//         r.customerID = CusID;
//         r.rating = atoi(RatingStr.c_str());
//         r.reviewText = RevText;

//         ReviewNode* node = new ReviewNode(r);

//         if (!head) {
//             head = tail = node;
//         } else {
//             tail->next = node;
//             tail = node;
//         }


//         //cout << "[Review Added] " << r.productID << " - Rating: " << r.rating << endl;
//     }

//     return head;
// }