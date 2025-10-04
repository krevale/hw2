#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"

#include <map>
#include <fstream>
#include <string>

#include "mydatastore.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);

string getDirectory(const string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == string::npos) {
        return "";
    }
    return path.substr(0, pos + 1);
}

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    bool done = false;

    map<string, vector<Product*>> cartMap;
    User* currentUser = nullptr;
    vector<Product*>* currentCart = nullptr;

    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    string out = filename;
                    if (filename.find('/') == string::npos && filename.find('\\') == string::npos) {
                        out = getDirectory(argv[1]) + filename;
                    }

                    cerr << "WRITING DB TO: " << out << endl;
                    ofstream ofile(out.c_str());
                    if (!ofile) {
                        cout << "Invalid request" << endl;
                    } else {
                        ds.dump(ofile);
                        ofile.close();
                    }
                }
                done = true;
            }
            else if (cmd == "DUMP") {
                string filename;
                if(ss >> filename) {
                    string out = filename;
                    if (filename.find('/') == string::npos && filename.find('\\') == string::npos) {
                        out = getDirectory(argv[1]) + filename;
                    }

                    cerr << "WRITING DB TO: " << out << endl;
                    ofstream ofile(out.c_str());
                    if (!ofile) {
                        cout << "Invalid request" << endl;
                    } else {
                        ds.dump(ofile);
                        ofile.close();
                    }
                } else {
                    cout << "Invalid request" << endl;
                }
            }
	    /* Add support for other commands here */
            else if (cmd == "ADD") {
                string addUser;
                int index = 0;

                if (ss >> addUser) {
                    if (!ds.userExists(addUser)) {
                        cout << "Invalid request" << endl;
                        continue;
                    }

                    if (ss >> index) {
											int idx = index - 1;
											if (idx < 0 || idx >= static_cast<int>(hits.size())) {
												cout << "Invalid request" << endl;
												continue;
											}
                        currentUser = ds.returnUser(addUser);

                        currentCart = &cartMap[addUser];

												Product* tmp = hits[idx];
                        currentCart->push_back(tmp);
                        
                    } else {
                        cout << "Invalid request" << endl;
                    }
                } else {
                    cout << "Invalid request" << endl;
                }
            }
            else if (cmd == "VIEWCART") {
                string viewUser;

                if (ss >> viewUser) {
                    if (!ds.userExists(viewUser)) {
                        cout << "Invalid username" << endl;
                        continue;
                    }

                    currentUser = ds.returnUser(viewUser);

                    currentCart = &cartMap[viewUser];

                    for (size_t i = 0; i < currentCart->size(); i++) {
                        Product* currentItem = (*currentCart)[i];
                        cout << "Item " << i+1 << endl;
                        cout << currentItem->displayString() << endl;
                    }
                } else {
                    cout << "Invalid username" << endl;
                }
            }
            else if (cmd == "BUYCART") {
                string viewUser;

                if (ss >> viewUser) {
                    if (!ds.userExists(viewUser)) {
                        cout << "Invalid username" << endl;
                        continue;
                    }

                    currentUser = ds.returnUser(viewUser);
										currentCart = &cartMap[viewUser];
										size_t index = 0;
                                        vector<Product*> remaining;

                    while (currentUser->getBalance() > 0 && index < (*currentCart).size()) {
											Product* currentItem = (*currentCart)[index];
											if (currentItem->getQty() > 0 && currentUser->getBalance() >= currentItem->getPrice()) {
												currentUser->deductAmount(currentItem->getPrice());
												currentItem->subtractQty(1);
											} else {
                                                remaining.push_back(currentItem);
                                            }
											index++;
										}
                    currentCart->swap(remaining);
                } else {
                    cout << "Invalid username" << endl;
                }
            }


            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if (hits.begin() == hits.end()) {
    	cout << "No results found!" << endl;
    	return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}
