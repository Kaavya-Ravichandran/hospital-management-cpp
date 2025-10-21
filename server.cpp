#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// Download cpp-httplib from: https://github.com/yhirose/cpp-httplib
// Place httplib.h in the same directory as this file
#include "httplib.h"
#include "json.hpp" // Download from: https://github.com/nlohmann/json

using json = nlohmann::json;
using namespace std;
using namespace httplib;

class Patient {
public:
    int id;
    string name;
    int age;
    string gender;
    string contact;
    string disease;
    string admitDate;
    string doctor;
    string room;

    Patient() : id(0), age(0) {}
    
    Patient(int i, string n, int a, string g, string c, string d, string date, string doc, string r)
        : id(i), name(n), age(a), gender(g), contact(c), disease(d), admitDate(date), doctor(doc), room(r) {}

    json toJson() const {
        return json{
            {"id", id},
            {"name", name},
            {"age", age},
            {"gender", gender},
            {"contact", contact},
            {"disease", disease},
            {"admitDate", admitDate},
            {"doctor", doctor},
            {"room", room}
        };
    }

    static Patient fromJson(const json& j) {
        Patient p;
        p.id = j.value("id", 0);
        p.name = j.value("name", "");
        p.age = j.value("age", 0);
        p.gender = j.value("gender", "");
        p.contact = j.value("contact", "");
        p.disease = j.value("disease", "");
        p.admitDate = j.value("admitDate", "");
        p.doctor = j.value("doctor", "");
        p.room = j.value("room", "");
        return p;
    }

    void saveToFile(ofstream &file) const {
        file << id << "|" << name << "|" << age << "|" << gender << "|" 
             << contact << "|" << disease << "|" << admitDate << "|" 
             << doctor << "|" << room << endl;
    }

    static Patient loadFromFile(const string& line) {
        Patient p;
        stringstream ss(line);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 9) {
            p.id = stoi(tokens[0]);
            p.name = tokens[1];
            p.age = stoi(tokens[2]);
            p.gender = tokens[3];
            p.contact = tokens[4];
            p.disease = tokens[5];
            p.admitDate = tokens[6];
            p.doctor = tokens[7];
            p.room = tokens[8];
        }
        return p;
    }
};

class Hospital {
private:
    vector<Patient> patients;
    int nextId;
    string filename;

public:
    Hospital(string file = "hospital_data.txt") : nextId(1001), filename(file) {
        loadFromFile();
    }

    ~Hospital() {
        saveToFile();
    }

    Patient addPatient(const json& patientData) {
        Patient p = Patient::fromJson(patientData);
        p.id = nextId++;
        patients.push_back(p);
        saveToFile();
        return p;
    }

    vector<Patient> getAllPatients() const {
        return patients;
    }

    Patient* getPatientById(int id) {
        for (auto& p : patients) {
            if (p.id == id) {
                return &p;
            }
        }
        return nullptr;
    }

    vector<Patient> searchPatients(const string& query) const {
        vector<Patient> results;
        string lowerQuery = query;
        transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        for (const auto& p : patients) {
            string lowerName = p.name;
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            
            if (to_string(p.id).find(query) != string::npos || 
                lowerName.find(lowerQuery) != string::npos) {
                results.push_back(p);
            }
        }
        return results;
    }

    bool dischargePatient(int id) {
        auto it = find_if(patients.begin(), patients.end(), 
            [id](const Patient& p) { return p.id == id; });
        
        if (it != patients.end()) {
            patients.erase(it);
            saveToFile();
            return true;
        }
        return false;
    }

    json getStatistics() const {
        int total = patients.size();
        int male = 0, female = 0, other = 0;

        for (const auto& p : patients) {
            if (p.gender == "Male") male++;
            else if (p.gender == "Female") female++;
            else other++;
        }

        return json{
            {"total", total},
            {"male", male},
            {"female", female},
            {"other", other}
        };
    }

    void saveToFile() const {
        ofstream file(filename);
        if (!file) {
            cerr << "Error opening file for writing!" << endl;
            return;
        }

        file << nextId << endl;
        for (const auto& p : patients) {
            p.saveToFile(file);
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file) {
            return;
        }

        file >> nextId;
        file.ignore();

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                Patient p = Patient::loadFromFile(line);
                if (p.id != 0) {
                    patients.push_back(p);
                }
            }
        }
        file.close();
    }
};

int main() {
    Hospital hospital;
    Server svr;

    // Serve static frontend files from current directory (index.html, etc.)
    svr.set_base_dir(".");

    // Enable CORS for all routes
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    // Handle OPTIONS requests for CORS
    svr.Options("/(.*)", [](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    // Serve index at root
    svr.Get("/", [&](const Request& req, Response& res) {
        // Redirect to index.html so that opening '/' shows the UI
        res.set_redirect("/index.html");
    });

    // GET all patients
    svr.Get("/api/patients", [&](const Request& req, Response& res) {
        json response = json::array();
        for (const auto& p : hospital.getAllPatients()) {
            response.push_back(p.toJson());
        }
        res.set_content(response.dump(), "application/json");
    });

    // GET patient by ID
    svr.Get(R"(/api/patients/(\d+))", [&](const Request& req, Response& res) {
        int id = stoi(req.matches[1]);
        Patient* patient = hospital.getPatientById(id);
        
        if (patient) {
            res.set_content(patient->toJson().dump(), "application/json");
        } else {
            json error = {{"error", "Patient not found"}};
            res.status = 404;
            res.set_content(error.dump(), "application/json");
        }
    });

    // POST add new patient
    svr.Post("/api/patients", [&](const Request& req, Response& res) {
        try {
            json patientData = json::parse(req.body);
            Patient newPatient = hospital.addPatient(patientData);
            
            json response = {
                {"message", "Patient added successfully"},
                {"patient", newPatient.toJson()}
            };
            res.status = 201;
            res.set_content(response.dump(), "application/json");
        } catch (const exception& e) {
            json error = {{"error", e.what()}};
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    // DELETE discharge patient
    svr.Delete(R"(/api/patients/(\d+))", [&](const Request& req, Response& res) {
        int id = stoi(req.matches[1]);
        
        if (hospital.dischargePatient(id)) {
            json response = {{"message", "Patient discharged successfully"}};
            res.set_content(response.dump(), "application/json");
        } else {
            json error = {{"error", "Patient not found"}};
            res.status = 404;
            res.set_content(error.dump(), "application/json");
        }
    });

    // GET search patients
    svr.Get("/api/patients/search", [&](const Request& req, Response& res) {
        string query = req.get_param_value("q");
        vector<Patient> results = hospital.searchPatients(query);
        
        json response = json::array();
        for (const auto& p : results) {
            response.push_back(p.toJson());
        }
        res.set_content(response.dump(), "application/json");
    });

    // GET statistics
    svr.Get("/api/statistics", [&](const Request& req, Response& res) {
        json stats = hospital.getStatistics();
        res.set_content(stats.dump(), "application/json");
    });

    cout << "🏥 Hospital Management System API Server" << endl;
    cout << "==========================================" << endl;
    cout << "Server running at: http://localhost:8080" << endl;
    cout << "API Endpoints:" << endl;
    cout << "  GET    /api/patients           - Get all patients" << endl;
    cout << "  GET    /api/patients/:id       - Get patient by ID" << endl;
    cout << "  POST   /api/patients           - Add new patient" << endl;
    cout << "  DELETE /api/patients/:id       - Discharge patient" << endl;
    cout << "  GET    /api/patients/search?q= - Search patients" << endl;
    cout << "  GET    /api/statistics         - Get statistics" << endl;
    cout << "\nPress Ctrl+C to stop the server..." << endl;

    svr.listen("0.0.0.0", 8080);

    return 0;
}
