#include "WebServer.h"

void handleSubmit(AsyncWebServerRequest *request)
{
    if (request->hasParam("time"))
    {
        if (dosage->singleDoseSelected)
        {
            dosage->singleDoseTime = request->getParam("time")->value().toDouble();
            Serial.println("Changed single dose time");
        }
        else
        {
            dosage->doubleDoseTime = request->getParam("time")->value().toDouble();
            Serial.println("Changed double");
        }
        dosage->writeToEEPROM();
    }

    if (request->hasParam("mode")){
        dosage->singleDoseSelected = !dosage->singleDoseSelected;
        Serial.println("Switched mode");
    }

    request->redirect("/");
}

void handleRootPage(AsyncWebServerRequest *request)
{
    String htmlPage = "<html><body>";
    htmlPage += "<h1>Select Mode and Time</h1>";
    htmlPage += "<form method='get' action='/submit'>";
    if (dosage->singleDoseSelected)
    {
        htmlPage += "<h2>Single Dose</h2>";
        htmlPage += "Time (seconds): <input type='text' name='time' value='" + String(dosage->singleDoseTime) + "'><br>";
    }
    else
    {
        htmlPage += "<h2>Double Dose</h2>";
        htmlPage += "Time (seconds): <input type='text' name='time' value='" + String(dosage->doubleDoseTime) + "'><br>";
    }
    htmlPage += "<input type='submit' value='Submit'>";
    htmlPage += "</form>";

    htmlPage += "<form method='get' action='/submit'>";
    htmlPage += "<button type='submit' name='mode'>Switch Mode</button>";
    htmlPage += "</form>";
    htmlPage += "</body></html>";

    request->send(200, "text/html", htmlPage);
}