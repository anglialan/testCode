int main(int argc, char *argv[]) {
	char ** training;
	int * trainingSize;
	if (argc > 1) {
		//Use command line argument as filename
		char* filename=argv[1];
		//seed a random number for shuffling
		srand(time(0));
		vector<Example> training = parseFile(filename);
		vector <Example> trainingBound = training;
		
		//shuffle 
		random_shuffle(training.begin(), training.end());

		vector<Example> testing;
		//Split vector into training 80% and testing 20%
		int cutoff = training.size() * 0.2;
		//cutoff = 5;
		//Move 20% into testing
		for (int i=0; i<cutoff; ++i) {
			testing.push_back(training.at(i));
		}
		//Remove duplicates in training
		training.erase(training.begin(), training.begin()+cutoff);
		//Vector for all attributes 1-22
		vector<int> attr;
		for (int i=1; i<23; ++i) {
			attr.push_back(i);
		}
		//Empty vector for parentExamples
		vector<Example> parent;


		//Create the tree for unbound depth
		treeNode * tree = dtLearning(training, attr, parent, 1, 0);
		//bool isCorrect = checkExample(tree, testing.at(0));
		int countCorrect= 0;
		
		//Display
		cout <<endl << left << setw(11)<< "unbound" <<setw(11)
			<<"train%"<<setw(11)<< "test%"<< endl;
		
		//Perform tests on each training example
		for (int i =0; i<training.size() ; ++i ) {
			//Count number of correct 
			if(checkExample(tree, training.at(i))) {
				++countCorrect;
			}
		}
		double unboundAccuracy = static_cast<double>(countCorrect) /
			 training.size() *100;
		//Display
		cout << left << setw(11)<<"   "<<setw(11)<<unboundAccuracy;
		countCorrect= 0;
		//Perform tests on each test example
		for (int i =0; i<testing.size() ; ++i ) {
			//Count number of correct 
			if(checkExample(tree, testing.at(i))) {
				++countCorrect;
			}
		}
		unboundAccuracy = static_cast<double>(countCorrect) /
			 testing.size() *100;
		cout <<setw(11)<<unboundAccuracy << endl << endl;
		// Mem management
		deleteTree(tree);

		//Bounded decision tree
		//Separate training, validation, testing
		random_shuffle(trainingBound.begin(), trainingBound.end());
		//Reset testing
		testing.clear();
		//Move 20% into testing
		for (int i=0; i<cutoff; ++i) {
			testing.push_back(trainingBound.at(i));
		}
		//Remove duplicates in trainingBound
		trainingBound.erase(trainingBound.begin(), 
					trainingBound.begin()+cutoff);
		//Make copy of 80% to use for testing best bound
		vector <Example> trainingBest = trainingBound;
		//Remove another 60% for validation
		vector <Example> validation;
		//Move 20% into validation
		for (int i=0; i<cutoff; ++i) {
			validation.push_back(trainingBound.at(i));
		}
		//Remove duplicates in trainingBound
		trainingBound.erase(trainingBound.begin(), 
					trainingBound.begin()+cutoff);
		//Check depth bound 1-15
		cout <<setw(11)<< "depth"<< setw(11)<<"train%"
			<< setw(11)<<"valid%" << endl;
		for (int i=1; i<16; ++i) {
			treeNode * boundTree = dtLearning(trainingBound, 
										attr, parent, 1, i);
			// denote optimum with *
			if (i ==4 ) {
				cout <<setw(11)<< "4*";
			} else {
				cout <<setw(11)<< i;

			}
			countCorrect = 0;
			//Perform tests on each training example
			for (int i =0; i<trainingBound.size() ; ++i ) {
				//Count number of correct 
				if(checkExample(boundTree, trainingBound.at(i))) {
					++countCorrect;
				}
			}
			double boundAccuracy = static_cast<double>(countCorrect) /
			 	trainingBound.size() * 100;
			cout <<setw(11)<< boundAccuracy;
			countCorrect = 0;
			//Perform tests on each validation example
			for (int i =0; i<validation.size() ; ++i ) {
				//Count number of correct 
				if(checkExample(boundTree, validation.at(i))) {
					++countCorrect;
				}
			}
			boundAccuracy = static_cast<double>(countCorrect) /
			 	validation.size() * 100;
			cout <<setw(11)<< boundAccuracy << endl;
			deleteTree(boundTree);

		}
		//Optimal depth is 4
		tree = dtLearning(trainingBest, attr, parent, 1, 4);
		countCorrect = 0;
		//Perform tests on trainging 
		for (int i =0; i<trainingBest.size() ; ++i ) {
			//Count number of correct 
			if(checkExample(tree, trainingBest.at(i))) {
				++countCorrect;
			}
		}
		double bestAccuracy = static_cast<double>(countCorrect) /
			 trainingBest.size() * 100;
		cout <<endl << setw(11) << "OPT=4" <<
		 	setw(11)<<"train%"<< setw(11)<<"test%" << endl;
		cout << setw(11) << " " << setw(11)<< bestAccuracy;
		countCorrect = 0;
		//Perform tests on testing 
		for (int i =0; i<testing.size() ; ++i ) {
			//Count number of correct 
			if(checkExample(tree, testing.at(i))) {
				++countCorrect;
			}
		}
		bestAccuracy = static_cast<double>(countCorrect) /
			 testing.size() * 100;
		cout << setw(11) << bestAccuracy <<endl;
		deleteTree(tree);
	} else {
		cout << "Error, no filename entered" << endl;
	}
}