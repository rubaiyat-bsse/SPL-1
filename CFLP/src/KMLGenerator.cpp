#include "KMLGenerator.hpp"
#include <fstream>
#include <iostream>

using namespace std;

void generateKML(const vector<Facility>& facilities, 
                 const vector<Customer>& customers, 
                 const vector<int>& bestY, 
                 const vector<vector<double>>& bestX, 
                 const string& filename){
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << " for writing KML.\n";
        return;
    }

    // KML XML Header
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n";
    file << "<Document>\n";
    file << "  <name>Optimized CFLP Solution</name>\n";

    // define Styles for the map
    // red pushpin for facilities
    file << "  <Style id=\"facilityStyle\">\n";
    file << "    <IconStyle>\n";
    file << "      <color>ff0000ff</color>\n"; // KML color is aabbggrr (alpha, blue, green, red), this is Red
    file << "      <scale>1.5</scale>\n";
    file << "      <Icon><href>http://maps.google.com/mapfiles/kml/pushpin/red-pushpin.png</href></Icon>\n";
    file << "    </IconStyle>\n";
    file << "  </Style>\n";

    // blue pushpin for Customers
    file << "  <Style id=\"customerStyle\">\n";
    file << "    <IconStyle>\n";
    file << "      <color>ffff0000</color>\n"; // Blue in KML
    file << "      <scale>1.0</scale>\n";
    file << "      <Icon><href>http://maps.google.com/mapfiles/kml/pushpin/blue-pushpin.png</href></Icon>\n";
    file << "    </IconStyle>\n";
    file << "  </Style>\n";

    // greenish line for routes
    file << "  <Style id=\"lineStyle\">\n";
    file << "    <LineStyle>\n";
    file << "      <color>ffee82ee</color>\n"; // violate
    file << "      <width>2</width>\n";
    file << "    </LineStyle>\n";
    file << "  </Style>\n";

    // keep track of what facilities are actually active
    // write Opened Facilities
    for (int j = 0; j < facilities.size(); j++) {
        if (bestY[j] > 0) {
            file << "  <Placemark>\n";
            file << "    <name>Warehouse " << j << ": " << facilities[j].city.name << "</name>\n";
            file << "    <styleUrl>#facilityStyle</styleUrl>\n";
            file << "    <description>\n";
            file << "      Capacity: " << facilities[j].M << "\n";
            file << "      Fixed Cost: " << facilities[j].f << "\n";
            file << "    </description>\n";
            file << "    <Point>\n";
            file << "      <coordinates>" << facilities[j].city.lng << "," << facilities[j].city.lat << ",0</coordinates>\n";
            file << "    </Point>\n";
            file << "  </Placemark>\n";
        }
    }

    // write Customers
    for (int i = 0; i < customers.size(); i++) {
        file << "  <Placemark>\n";
        file << "    <name>Customer " << i << ": " << customers[i].city.name << "</name>\n";
        file << "    <styleUrl>#customerStyle</styleUrl>\n";
        file << "    <description>Demand: " << customers[i].d << "</description>\n";
        file << "    <Point>\n";
        file << "      <coordinates>" << customers[i].city.lng << "," << customers[i].city.lat << ",0</coordinates>\n";
        file << "    </Point>\n";
        file << "  </Placemark>\n";
    }

    // draw Lines connecting Customers to Opened Facilities representing Supply Flow
    for (int i = 0; i < customers.size(); i++) {
        for (int j = 0; j < facilities.size(); j++) {
            if (bestX[i][j] > 1e-6) {
                file << "  <Placemark>\n";
                file << "    <name>Route " << customers[i].city.name << " to " << facilities[j].city.name << "</name>\n";
                file << "    <description>Assigned Demand Flow: " << bestX[i][j] << "</description>\n";
                file << "    <styleUrl>#lineStyle</styleUrl>\n";
                file << "    <LineString>\n";
                file << "      <tessellate>1</tessellate>\n";
                file << "      <coordinates>\n";
                // formatting needs to strictly be longitude,latitude,altitude
                file << "        " << customers[i].city.lng << "," << customers[i].city.lat << ",0\n";
                file << "        " << facilities[j].city.lng << "," << facilities[j].city.lat << ",0\n";
                file << "      </coordinates>\n";
                file << "    </LineString>\n";
                file << "  </Placemark>\n";
            }
        }
    }

    // close the KML File XML tags
    file << "</Document>\n";
    file << "</kml>\n";

    file.close();
    cout << "KML Map successfully generated at: " << filename << "\n";
}