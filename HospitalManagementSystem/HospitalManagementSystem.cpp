#include <iostream>//For basic I/O;
#include "nlohmann/json.hpp"//For reading .json files and manipulating them;
#include <vector>//For temporarily storing and outputing sorted data;
#include <fstream>//For reading files;
#include <cstdlib>//For exiting console using client's input;
#include <string> //For casting to string;

using json = nlohmann::json;//Created an alias so that I don't have to use nlohmann::json each time;

class HospitalManagementSystem {//Used one class for entire application as it is quite a small application;
private:
	json data = readJSON();//Global variable is initialized as this app will not have a need for constant updates of data as it is small-scale;

	struct Patients {//Structure is needed for bubble sort later down the line;
		std::string name;
		std::string hospital;
		int id_number;
		double expenditure;
	};

public:
	void closeConsole() {
		exit(EXIT_SUCCESS);
	}
	void pauseScreen() {
		std::cout << std::endl;
		std::cout << "Press any button to continue...";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
	}
	json readJSON() {
		std::ifstream file("storage.json"); //Trying to read data using ifstream;
		json DATA; //That is where we store the data and using nlohmann/json library we can use the contents of .json file;
		if (!file.is_open())  {std::cerr << "Error: Could not open file 'storage.json'" << std::endl;closeConsole();}//If file wasn't open (doesn't exist or is corrupted) we throw this error output;
		else {
			try {
				file >> DATA;//We try to put buffer data from file into DATA, which will basically "understand" the .json file and help us work with it using c++;
				file.close();//Of course we close the file to reduce working memory.
				return DATA;
			}
			catch (const std::exception& e) {//In case the attempt failed, 
				std::cerr << "Error: Could not read data from file 'storage.json': " << e.what() << std::endl;//we throw this;
				file.close();//Of course we close it.
				closeConsole();//If we can't read data, there is no reason for the application to run anymore;
				return json();//Not necessary, but stil...;
			} 
		}
		file.close();//Just in case
		return DATA;
	}
	void detailsPanel(const int hospital_number, const int demanded_details) {//This will output whatever the client asked for
		const auto& hospital = data["hospitals"][hospital_number - 1];//1 less because, the input will be 1-3, while in programming and arrays we start counting from 0. So 0-2

		switch (demanded_details) {//switch-case for different output for each allowed demanded_details.
		case 1://DOCTORS
			for (const auto& doctor : hospital["doctors"]) std::cout << doctor << std::endl;
			break;
		case 2://PATIENTS
			for (const auto& patients : hospital["patients"]) {
				std::cout << "Patient's name: " << patients["name"] << std::endl;
				std::cout << "Patient's ID: " << patients["id_number"] << std::endl;
				std::cout << "Patient's expedinture: " << patients["patient_expenditure"] << " USD" << std::endl;
				std::cout << std::endl;
			}
			break;
		case 3://Beds Available
			std::cout << hospital["beds_available"] << std::endl;
			break;
		default:
			std::cout << demanded_details << "<- is wrong input or wasn't found!" << std::endl;//In case something weird happens and illegal input exists.
			break;
		}
		pauseScreen();
		returnToMain();
	}
	void sortByHospitals() {
		json hospitals = data["hospitals"];//Shows patients and their info from each hospital
		for (const auto& hospital : hospitals) { 
			json patients = hospital["patients"];
			std::cout << ">Patients from " << hospital["name"] << ": " << std::endl;
			for (const auto& patient : patients) {
				std::cout << "- NAME: " << patient["name"] << ", ID: " << patient["id_number"] << ", EXPENDITURE: " << patient["patient_expenditure"] << std::endl;
			}
			std::cout<<std::endl;
		}
	}
	void sortByExpedintures() { //Sorts patients from least to highest based on their expenditure and outputs their info. The .json file or JSON data is not affected. The vector containing same data is sorted;
		std::vector <Patients> PATIENTS;
		for (const auto& hospital : data["hospitals"]) {//Assings values to vector "expenditures" using struct Patients;
			for (const auto& patient : hospital["patients"]) {
				Patients temp;
				temp.name = (std::string)patient["name"];
				temp.expenditure = std::stod((std::string)patient["patient_expenditure"]);
				temp.id_number = std::stoi((std::string)patient["id_number"]);
				temp.hospital = (std::string)hospital["name"];
				PATIENTS.push_back(temp);
			}
		}
		for (int i = 0; i < PATIENTS.size(); i++) {//Bubble sort for expenditures of each patients;
			for (int b = i+1; b < PATIENTS.size(); b++) {// 1 2 4 7 8 9
				if (PATIENTS[i].expenditure > PATIENTS[b].expenditure) {
					int id = PATIENTS[i].id_number;
					double d = PATIENTS[i].expenditure;
					std::string s = PATIENTS[i].name;
					std::string h = PATIENTS[i].hospital;
					PATIENTS[i].expenditure = PATIENTS[b].expenditure;
					PATIENTS[i].name = PATIENTS[b].name;
					PATIENTS[i].id_number = PATIENTS[b].id_number;
					PATIENTS[i].hospital = PATIENTS[b].hospital;
					PATIENTS[b].expenditure = d;
					PATIENTS[b].name = s;
					PATIENTS[b].id_number = id;
					PATIENTS[b].hospital = h;
				}
			}
		}
		for (const auto& people : PATIENTS) {//Output the sorted vector;
			std::cout << "< EXPENDITURES: >" << people.expenditure <<"< HOSPITAL: >" << people.hospital << "---NAME: >" << people.name << "< ID: >" << people.id_number << "<" << std::endl;
		}
		returnToMain();
	}
	void sortByNames() {
		std::vector <Patients> PATIENTS;
		for (const auto& hospital : data["hospitals"]) {//Assings values to vector "expenditures" using struct Patients;
			for (const auto& patient : hospital["patients"]) {
				Patients temp;
				temp.name = (std::string)patient["name"];
				temp.expenditure = std::stod((std::string)patient["patient_expenditure"]);
				temp.id_number = std::stoi((std::string)patient["id_number"]);
				temp.hospital = (std::string)hospital["name"];
				PATIENTS.push_back(temp);
			}
		}
		std::cout << "What is the name of the patient you are looking for or his/her ID: ";
		std::string clientInput;//As it might be an int or string
		std::cin >> clientInput;
		int count = 0, match = 0;
		std::vector<Patients> matchingPatients; // Store matching patients
		for (const auto& patient : PATIENTS) {
			std::string theWord = patient.name;
			bool match = true; // Flag to indicate if patient matches client input
			int count = 0; // Reset count for each patient
			for (const char& piece : clientInput) {
				if (tolower(theWord[count]) != tolower(piece)) {
					match = false;
					break;// Break inner loop if a character doesn't match
				}
				count++;
			}
			if (match) {
				matchingPatients.push_back(patient);//Add matching patient to result
			}
		}
		PATIENTS = matchingPatients;//Update PATIENTS with only matching patients
		if (!PATIENTS.empty()) {
			std::cout << "All the matches are: " << std::endl;
			for (const auto& name : PATIENTS) {
				std::cout << ">NAME:" << name.name << "| ID:" << name.id_number << "| HOSPITAL:" << name.hospital << "| EXPENDITURES:" << name.expenditure << std::endl;
			}
		}
		returnToMain();
	}
	void peoplePanel() {//Shows all possible filters for patients;
		int dial = -1;
		while (dial != 1 && dial != 2 && dial != 3) {
			std::cout << "How do you want to filter all known patients over our network: " << std::endl;
			std::cout << "[1] By hospitals" << std::endl;
			std::cout << "[2] By expedintures" << std::endl;
			std::cout << "[3] By names" << std::endl;
			std::cin >> dial;
		}
		if (dial == 1) sortByHospitals();
		else if (dial == 2) sortByExpedintures();
		else sortByNames();
	}
	void hospitalsPanel() {//Shows what details you wish to see about any hospital you need;
		std::cout << "Hospital names: " << std::endl;
		int i = 1;
		for (json name : data["hospitals"]) {
			std::cout << "[" << i << "] " << name["name"] << std::endl;
			i++;
		}
		int hosp_num = -1; //Number of hospital, hence hosp_num;
		while(hosp_num < 1 || hosp_num >= i) {
			std::cout << "Which hospital's details do you wish to see: ";
			std::cin >> hosp_num;
		}
		std::cout << std::endl;
		int det_num = -1;//The input is a number about which detail you want to see from next 3, hence the name, det(detail)_num;
		while (det_num != 1 && det_num != 2 && det_num != 3) {
			std::cout << "[1] Doctors" << std::endl;
			std::cout << "[2] Patients" << std::endl;
			std::cout << "[3] Beds Available" << std::endl;
			std::cout << "What do you wish to see: ";
			std::cin >> det_num;
		}
		detailsPanel(hosp_num, det_num);
	}
	void returnToMain() {//Asks if you want to return to main menu or exit the application;
		int dial = -1;
		while (dial != 1 && dial != 2) {
			std::cout << "Do you wish to return to main menu: " << std::endl;
			std::cout << "[1] MAIN MENU " << std::endl;
			std::cout << "[2] Exit" << std::endl;
			std::cin >> dial;
		}
		if (dial == 1) mainMenu();
		else if (dial == 2) closeConsole();
	}
	void mainMenu() {//Main menu. Where the universe begins... LOL;
		int dial = -1;//The client wil DIAL the number so I thought DIAL was better than input IDUNNO...;
		while (dial != 1 && dial != 2 && dial != 3) {
			std::cout << "Which panel do you want to see: " << std::endl;
			std::cout << "[1] Hospitals " << std::endl;
			std::cout << "[2] Patients " << std::endl;
			std::cout << "[3] EXIT " << std::endl;
			std::cin >> dial;
		}
		if (dial == 1) hospitalsPanel();
		else if (dial == 2) peoplePanel();
		else closeConsole();
	}
};

int main()
{
	HospitalManagementSystem hms;//No need for dynamicall allocation of memory as the application is intended to run as one instance;
	hms.mainMenu();
}

/*Given data of Hospitals with the name of hospital,
patients and doctors and patients below are the
functionalities that needed to be implemented: 
(idea from) https://www.geeksforgeeks.org/hospital-management-system-in-c */