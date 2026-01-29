#include "Department.h"



Department::Department()
{
	Employees = new vector<Employee>();
	Programmers = new vector <Programmer>();
	SoftwareArchitects = new vector<SoftwareArchitect>();
	Name = "";
	ID = 0;
}

Department::Department(string N, int id)
{
	Employees = new vector<Employee>();
	Programmers = new vector <Programmer>();
	SoftwareArchitects = new vector<SoftwareArchitect>();
	Name = N;
	ID = (id > 0) ? id : 0;
}

double Department::CalculateMaxSalary(StaffTypes type)
{
	double max = 0;
	double i = 0;
	switch (type)
	{
	case Department::EMPLOYEE:
		for (i = 0; i < Employees->size(); i++)
		{

			if (Employees->at(i).getSalary()>max)
			{
				max = Employees->at(i).getSalary();
			}
		}
		break;
	case Department::PROGRAMMER:
		for (i = 0; i < Programmers->size(); i++)
		{

			if (Programmers->at(i).getSalary()>max)
			{
				max = Programmers->at(i).getSalary();
			}
		}
        break;
	case Department::SOFTWAREARCHITECTS:
		for (i = 0; i < SoftwareArchitects->size(); i++)
		{

			if (SoftwareArchitects->at(i).getSalary()>max)
			{
                max = SoftwareArchitects->at(i).getSalary();
			}
                // max = SoftwareArchitects->at(i).getSalary(); // extra line
		}
		break;
	default:
		break;
	}

	return max;
}

int Department::CalculateAverageSalary(StaffTypes type)
{
    double sum = 0.0;
	int i;

	switch (type)
	{
	case Department::EMPLOYEE:	
		if (Employees->size() == 0) return 0;	
		for (i = 0; i < Employees->size(); i++)
		{
			// fixed bug here this overwrite sum
			sum += Employees->at(i).getSalary();
		}
		return (int)(sum / Employees->size());
	case Department::PROGRAMMER:
		if (Programmers->size() == 0) return 0;
		for (i = 0; i < Programmers->size(); i++)
		{
			// fixed bug here += 
			sum += Programmers->at(i).getSalary();
		}
		return (int)(sum / Programmers->size());
	case Department::SOFTWAREARCHITECTS:
	if (SoftwareArchitects->size() == 0) return 0;
		for (i = 0; i < SoftwareArchitects->size(); i++)
		{
			sum += SoftwareArchitects->at(i).getSalary();
		}
		return(int)(sum / SoftwareArchitects->size());
	}

    return 0;
}

vector<Employee*> *Department::getAllStaff()
{
    vector<Employee*> *Result = new vector<Employee*>();
	for (int i = 0; i < Employees->size(); i++)
    {
        Result->push_back(&Employees->at(i));
	}
    for (int i = 0; i < Programmers->size(); i++)
    {
        Result->push_back(&Programmers->at(i));
    }
	for (int i = 0; i < SoftwareArchitects->size(); i++)
    {
        Result->push_back(&SoftwareArchitects->at(i));
	}

	return Result;
}

vector<Programmer> *Department::getProgrammersByLanguage(string Lang)
{
    vector<Programmer> *result = new vector<Programmer>();
	for (int i = 0; i < Programmers->size(); i++)
	{
		Programmer temp = Programmers->at(i);
        if (temp.getProgLanguage() == Lang)
		{
			result->push_back(temp);
		}
	}
	return result;
}

bool Department::RemoveEmployee(Employee E)
{
	for (int i = 0; i < Employees->size(); i++)
	{
        if (E.getID() == Employees->at(i).getID())
		{
			Employees->erase(Employees->begin() + i);
		}			
	}
	for (int i = 0; i <  Programmers->size(); i++)
	{
		if (E.getID() == Programmers->at(i).getID())
		{
			Programmers->erase(Programmers->begin() + i);
		}
	}
	for (int i = 0; i < SoftwareArchitects->size(); i++)
	{
		if (E.getID() == SoftwareArchitects->at(i).getID())
		{
			SoftwareArchitects->erase(SoftwareArchitects->begin() + i);
		}
	}
    return true;
}

vector<int>* Department::getAllProjectIDs()
{
	vector<int> *result = new vector<int>();
    int i;
    for ( i=0; i < Programmers->size(); i++)
	{
		// getProjectID was missing replaced getID with getProjectID
		if (!checkID(Programmers->at(i).getProjectID(), result))
		{
            result->push_back(Programmers->at(i).getProjectID());
		}
	}

    for (i=0; i < SoftwareArchitects->size(); i++)
	{
		if (!checkID(SoftwareArchitects->at(i).getID(), result))
		{
            result->push_back(SoftwareArchitects->at(i).getProjectID());
		}
	}
	return result;
}

bool Department::checkID(int ID, vector<int> *v)
{
	for (int i = 0; i < v->size(); i++)
	{
		if (v->at(i) == ID)
            return true;
	}
    return false;
}

Department::~Department()
{
    delete Employees;
    delete Programmers;
    delete SoftwareArchitects;
}


