/*
- This code is a part of the project "Water Treatment Plant Simulator" by FECORO
Updated: 2024/12

- 1. Introduction:
    - 1.1. What is a Wastewater Treatment System?
        - A wastewater treatment system is a system made up of several components that work together to treat wastewater from various sources. The goal of the system is to remove contaminants and pollutants from the water before it is discharged back into the environment.
    - 1.2. Purpose of the Project
        - The purpose of this project is to create a simulator for a wastewater treatment system. The simulator will allow users to design and simulate different treatment systems, monitor water quality parameters, and analyze the performance of the system.
    - 1.3. Project Overview
        - The project consists of several classes that represent different components of a wastewater treatment system. These components include:
            - Inlet: Represents the point where wastewater enters the system.
            - Outlet: Represents the point where treated water exits the system.
            - Pump: Represents a pump that moves water from one component to another.
            - FlowMeter: Represents a flow meter that measures the flow rate of water.
            - Tank: Represents a tank that stores water for a certain period of time.
            - Reactor: Represents a reactor that treats water by removing contaminants.
            - Filter: Represents a filter that removes suspended solids from water.
            - DisinfectionUnit: Represents a unit that disinfects water to remove pathogens.
            - Clarifier: Represents a clarifier that separates solids from water.
            - SludgeDryingBed: Represents a sludge drying bed that dries sludge for disposal.
        - The project also includes a Connection class that represents the connections between components. The Connection class is responsible for moving water between components and simulating the flow of water through the system.

- 2. Code Explanation:
    - 2.1. Component Class:
        - The Component class is the base class for all components in the system. It contains common attributes and methods that are shared by all components.
        - The Component class has the following attributes:
            - name: A string that represents the name of the component.
            - description: A string that provides a description of the component.
            - position: A vector that represents the position of the component in the system.
            - shape: A rectangle shape that represents the visual representation of the component.
            - width: The width of the component.
            - height: The height of the component.
            - volume: The volume of water stored in the component.
            - flowRate: The flow rate of water through the component.
        - The Component class has the following methods:
            - simulate(deltaTime): A virtual method that simulates the behavior of the component over a time step.
            - update(deltaTime): A virtual method that updates the visual representation of the component.
            - draw(window): A virtual method that draws the component on the screen.
    - 2.2. Inlet and Outlet Classes:
        - The Inlet and Outlet classes are subclasses of the Component class. They represent the points where water enters and exits the system, respectively.
        - The Inlet class has a green color, while the Outlet class has a red color to distinguish them visually.
    - 2.3. Connection Class:
        - The Connection class represents the connection between two components in the system. It is responsible for moving water from one component to another and simulating the flow of water through the system.
        - The Connection class has the following attributes:
            - from: A pointer to the component where water is coming from.
            - to: A pointer to the component where water is going to.
            - flowParticles: A vector of circle shapes that represent the flow of water particles between components.
            - particleSpawnTime: A float that tracks the time since the last water particle was spawned.
        - The Connection class has the following methods:
            - update(deltaTime): Updates the flow of water particles between components.
            - draw(window): Draws the connection between components on the screen.
    - 2.4. Main Function:
        - The main function initializes the simulation window and sets up the ImGui interface for user interaction.
        - It creates the initial components (Inlet and Outlet) and adds
        - It sets up variables for adding new components, simulating the system, and tracking the simulation speed.
        - It runs the simulation loop, updating and drawing the components and connections on the screen.
        - It handles user input through the ImGui interface, allowing users to add new components and control the simulation.
        - It cleans up resources and exits the program when the window is closed.

- 3. Conclusion:
    - This code snippet provides the foundation for a wastewater treatment plant simulator. It defines the basic classes and functionality needed to create a system of interconnected components that simulate the treatment of wastewater. By expanding on this code, additional components, interactions, and visualizations can be added to create a more comprehensive and realistic simulation of a water treatment system.

Made by FERI.
*/

//|........||main.cpp

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <map>
#include <algorithm>
#include <deque> //..Include deque for storing water quality history
#include <memory>

//|........||Definition of constants and enumerations
const float SIMULATION_TIME_STEP = 0.016f; //|........||Simulation time step in seconds

enum WaterParameter {
    BOD,        //|........||Biochemical Oxygen Demand
    COD,        //|........||Chemical Oxygen Demand
    TSS,        //|........||Total Suspended Solids
    NH4,        //|........||Ammonium
    NO3,        //|........||Nitrate
    PH,         //|........||pH Level
    P,          //|........||Total Phosphorus
    OIL,        //|........||Oils and Greases
    DO,         //|........||Dissolved Oxygen
    TEMP,       //|........||Temperature
    PATHOGENS,  //|........||Pathogens
    SALINITY,   //|........||Salinity
    TURBIDITY,  //|........||Turbidity
    EC,         //|........||Electrical Conductivity
    ALKALINITY, //|........||Alkalinity
    RESIDUAL_CHLORINE, //|........||Residual Chlorine
    HARDNESS,   //|........||Hardness
    SULFATES,   //|........||Sulfates
    CHLORIDES,  //|........||Chlorides
    METALS      //|........||Heavy Metals
};

//|........||Helper function to convert parameters to string
std::string parameterToString(WaterParameter param) {
    switch (param) {
        case BOD: return "BOD (mg/L)";
        case COD: return "COD (mg/L)";
        case TSS: return "TSS (mg/L)";
        case NH4: return "NH₄⁺ (mg/L)";
        case NO3: return "NO₃⁻ (mg/L)";
        case PH: return "pH";
        case P: return "Total Phosphorus (mg/L)";
        case OIL: return "Oils and Greases (mg/L)";
        case DO: return "Dissolved Oxygen (mg/L)";
        case TEMP: return "Temperature (°C)";
        case PATHOGENS: return "Pathogens (CFU/mL)";
        case SALINITY: return "Salinity (ppt)";
        case TURBIDITY: return "Turbidity (NTU)";
        case EC: return "Electrical Conductivity (µS/cm)";
        case ALKALINITY: return "Alkalinity (mg CaCO₃/L)";
        case RESIDUAL_CHLORINE: return "Residual Chlorine (mg/L)";
        case HARDNESS: return "Total Hardness (mg CaCO₃/L)";
        case SULFATES: return "Sulfates (mg/L)";
        case CHLORIDES: return "Chlorides (mg/L)";
        case METALS: return "Heavy Metals (mg/L)";
        default: return "";
    }
}

//...................................................................................................

//|........||Class to represent water and its parameters
class Water {
public:
    std::map<WaterParameter, float> parameters;

    Water() {
        //|........||Default initial values
        parameters[BOD] = 300.0f;
        parameters[COD] = 600.0f;
        parameters[TSS] = 200.0f;
        parameters[NH4] = 50.0f;
        parameters[NO3] = 5.0f;
        parameters[PH] = 6.5f;
        parameters[P] = 10.0f;
        parameters[OIL] = 30.0f;
        parameters[DO] = 2.0f;
        parameters[TEMP] = 20.0f;
        parameters[PATHOGENS] = 1e6f;
        parameters[SALINITY] = 0.5f;
        parameters[TURBIDITY] = 50.0f;
        parameters[EC] = 1500.0f;
        parameters[ALKALINITY] = 200.0f;
        parameters[RESIDUAL_CHLORINE] = 0.0f;
        parameters[HARDNESS] = 250.0f;
        parameters[SULFATES] = 80.0f;
        parameters[CHLORIDES] = 100.0f;
        parameters[METALS] = 5.0f;
    }

    void updateParameter(WaterParameter param, float value) {
        parameters[param] = value;
    }

    float getParameter(WaterParameter param) {
        return parameters[param];
    }
};

//|........||Base class for system components
class Component {
public:
    std::string name;
    std::string description;
    sf::Vector2f position;
    sf::RectangleShape shape;
    //...Other shape for inlet and outlet
    sf::CircleShape inletShape;
    sf::CircleShape outletShape;
    float width = 120;
    float height = 120;

    //|........||Component parameters
    float volume = 1000;     //|........||in m³
    float flowRate = 100;    //|........||in m³/day
    float HRT = 10;          //|........||Hydraulic Retention Time in hours
    float SRT = 20;          //|........||Solids Retention Time in days
    float temperature = 20.0f; //|........||Temperature in °C

    //|........||Inlet and outlet water parameters
    Water inletWater;
    Water outletWater;

    //|........||Animation variables
    std::vector<sf::CircleShape> waterParticles;
    float particleSpawnTime = 0.0f;

    //|........||Removal efficiencies for different water parameters
    std::map<WaterParameter, float> removalEfficiencies;

    Component(const std::string& n, const std::string& desc, const sf::Vector2f& pos)
        : name(n), description(desc), position(pos) {
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(position);
        shape.setFillColor(sf::Color(100, 100, 250)); //|........||Base color
        shape.setOutlineThickness(2.0f);
        shape.setOutlineColor(sf::Color::Black);
    }

    virtual void simulate(float deltaTime) {
        outletWater = inletWater;
    }

    virtual void update(float deltaTime) {
        particleSpawnTime += deltaTime;
        if (particleSpawnTime >= 0.05f) {
            particleSpawnTime = 0.0f;
            sf::CircleShape particle(5.0f);
            particle.setFillColor(sf::Color(0, 255, 255));
            particle.setPosition(position.x + width / 2, position.y + height / 2);
            waterParticles.push_back(particle);
        }

        for (auto& particle : waterParticles) {
            float speed = 50.0f; //|........||Particle movement speed
            particle.move(speed * deltaTime, 0.0f);
            float angle = std::sin(particle.getPosition().x * 0.05f) * 5.0f;
            particle.setRotation(angle);
        }

        waterParticles.erase(std::remove_if(waterParticles.begin(), waterParticles.end(),
            [this](sf::CircleShape& p) {
                return p.getPosition().x > position.x + width;
            }), waterParticles.end());
    }

    virtual void draw(sf::RenderWindow& window) {
        window.draw(shape);
        for (auto& particle : waterParticles) {
            window.draw(particle);
        }
    }

    void addRemovalEfficiency(WaterParameter param, float efficiency) {
        removalEfficiencies[param] = efficiency;
    }

    void removeRemovalEfficiency(WaterParameter param) {
        removalEfficiencies.erase(param);
    }
};

//|........||Inlet component of the system
class Inlet : public Component {
public:
    Inlet(const sf::Vector2f& pos) : Component("Inlet", "Entry point of wastewater into the system.", pos) {
        inletShape.setFillColor(sf::Color::Green);
    }
};

//|........||Outlet component of the system
class Outlet : public Component {
public:
    Outlet(const sf::Vector2f& pos) : Component("Outlet", "Exit point of treated water from the system.", pos) {
        outletShape.setFillColor(sf::Color::Red);
    }
};

//|........||Primary Clarifier: removes settleable solids and oil & grease
class PrimaryClarifier : public Component {
public:
    PrimaryClarifier(const sf::Vector2f& pos) : Component(
        "Primary Clarifier",
        "Removes settleable solids and oil & grease from wastewater.",
        pos) {
        shape.setFillColor(sf::Color(210, 105, 30));
    }

    void simulate(float deltaTime) override {
        float removalEfficiencyTSS = 0.70f;
        float removalEfficiencyOIL = 0.90f;
        outletWater = inletWater;
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) * (1 - removalEfficiencyTSS));
        outletWater.updateParameter(OIL, inletWater.getParameter(OIL) * (1 - removalEfficiencyOIL));
        outletWater.updateParameter(TURBIDITY, inletWater.getParameter(TURBIDITY) * 0.8f);
    }
};

//|........||Electrocoagulation Unit: removes metals and suspended solids, changes EC and pH
class ElectrocoagulationUnit : public Component {
public:
    ElectrocoagulationUnit(const sf::Vector2f& pos) : Component(
        "Electrocoagulation Unit",
        "Removes metals and suspended solids, changes EC and pH of wastewater.",
        pos) {
        shape.setFillColor(sf::Color(255, 215, 0));
    }

    void simulate(float deltaTime) override {
        float removalEfficiencyMETALS = 0.80f;
        float removalEfficiencyTSS = 0.60f;
        float pH_adjustment = 0.5f;
        float EC_adjustment = 200.0f;
        outletWater = inletWater;
        outletWater.updateParameter(METALS, inletWater.getParameter(METALS) * (1 - removalEfficiencyMETALS));
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) * (1 - removalEfficiencyTSS));
        outletWater.updateParameter(PH, inletWater.getParameter(PH) + pH_adjustment);
        outletWater.updateParameter(EC, inletWater.getParameter(EC) + EC_adjustment);
    }
};

//|........||Filtration: Removes suspended solids and turbidity
class Filtration : public Component {
public:
    Filtration(const sf::Vector2f& pos) : Component(
        "Filtration",
        "Removes suspended solids and turbidity from wastewater.",
        pos) {
        shape.setFillColor(sf::Color(192, 192, 192));
    }

    void simulate(float deltaTime) override {
        float removalEfficiencyTSS = 0.80f;
        float removalEfficiencyTURB = 0.70f;
        outletWater = inletWater;
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) * (1 - removalEfficiencyTSS));
        outletWater.updateParameter(TURBIDITY, inletWater.getParameter(TURBIDITY) * (1 - removalEfficiencyTURB));
    }
};

//Anaeroic-Aerobic Filter: removes BOD, COD, and NH4 through biological treatment
class AnaerobicAerobicFilter : public Component {
public:
    AnaerobicAerobicFilter(const sf::Vector2f& pos) : Component(
        "Anaerobic-Aerobic Filter",
        "Biological treatment system to remove BOD, COD, and NH₄⁺ from wastewater.",
        pos) {
        shape.setFillColor(sf::Color(0, 128, 128));
    }

    void simulate(float deltaTime) override {
        float k_bod = 0.2f;
        float k_cod = 0.1f;
        float k_nh4 = 0.05f;
        float tempFactor = std::pow(1.035f, inletWater.getParameter(TEMP) - 20.0f);

        float BOD_in = inletWater.getParameter(BOD);
        float COD_in = inletWater.getParameter(COD);
        float NH4_in = inletWater.getParameter(NH4);

        float BOD_removal = BOD_in * (1 - std::exp(-k_bod * HRT * tempFactor));
        float COD_removal = COD_in * (1 - std::exp(-k_cod * HRT * tempFactor));
        float NH4_removal = NH4_in * (1 - std::exp(-k_nh4 * HRT * tempFactor));

        outletWater = inletWater;
        outletWater.updateParameter(BOD, BOD_in - BOD_removal);
        outletWater.updateParameter(COD, COD_in - COD_removal);
        outletWater.updateParameter(NH4, NH4_in - NH4_removal);
        outletWater.updateParameter(NO3, inletWater.getParameter(NO3) + NH4_removal * 0.9f);

        float DO_consumed = (BOD_removal + COD_removal + NH4_removal * 4.57f) * 1.5f;
        outletWater.updateParameter(DO, inletWater.getParameter(DO) - DO_consumed);
        if (outletWater.getParameter(DO) < 0) outletWater.updateParameter(DO, 0);
    }
};


//|........||Ozone Disinfection: removes pathogens and oxidizes contaminants
class OzoneDisinfection : public Component {
public:
    OzoneDisinfection(const sf::Vector2f& pos) : Component(
        "Ozone Disinfection",
        "Removes pathogens and oxidizes contaminants using ozone.",
        pos) {
        shape.setFillColor(sf::Color(255, 255, 0));
    }

    void simulate(float deltaTime) override {
        float pathogen_removal = inletWater.getParameter(PATHOGENS) * 0.999f;
        float oxidationEfficiency = 0.90f;
        outletWater = inletWater;
        outletWater.updateParameter(PATHOGENS, inletWater.getParameter(PATHOGENS) - pathogen_removal);
        outletWater.updateParameter(COD, inletWater.getParameter(COD) * (1 - oxidationEfficiency));
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) * (1 - oxidationEfficiency));
    }
};

//|........||MBR: Membrane Bioreactor for advanced treatment
class MBR : public Component {
public:
    MBR(const sf::Vector2f& pos) : Component(
        "MBR",
        "Membrane bioreactor, bacteria and protozoa remove contaminants.",
        pos) {
        shape.setFillColor(sf::Color(128, 128, 128));
    }

    void simulate(float deltaTime) override {
        float k_bod = 0.1f;
        float k_cod = 0.05f;
        float k_nh4 = 0.03f;
        float tempFactor = std::pow(1.035f, inletWater.getParameter(TEMP) - 20.0f);

        float BOD_in = inletWater.getParameter(BOD);
        float COD_in = inletWater.getParameter(COD);
        float NH4_in = inletWater.getParameter(NH4);

        float BOD_removal = BOD_in * (1 - std::exp(-k_bod * HRT * tempFactor));
        float COD_removal = COD_in * (1 - std::exp(-k_cod * HRT * tempFactor));
        float NH4_removal = NH4_in * (1 - std::exp(-k_nh4 * HRT * tempFactor));

        outletWater = inletWater;
        outletWater.updateParameter(BOD, BOD_in - BOD_removal);
        outletWater.updateParameter(COD, COD_in - COD_removal);
        outletWater.updateParameter(NH4, NH4_in - NH4_removal);
        outletWater.updateParameter(NO3, inletWater.getParameter(NO3) + NH4_removal * 0.9f);

        float DO_consumed = (BOD_removal + COD_removal + NH4_removal * 4.57f) * 1.5f;
        outletWater.updateParameter(DO, inletWater.getParameter(DO) - DO_consumed);
        if (outletWater.getParameter(DO) < 0) outletWater.updateParameter(DO, 0);
    }
};

//|........||Realistic Biofilter: removes BOD, COD, and NH4 through biological treatment
class Biofilter : public Component {
public:
    Biofilter(const sf::Vector2f& pos) : Component(
        "Realistic Biofilter",
        "Biological treatment system to remove BOD, COD, and NH₄⁺ from wastewater.",
        pos) {
        shape.setFillColor(sf::Color(0, 128, 0));
    }

    void simulate(float deltaTime) override {
        float k_bod = 0.2f;
        float k_cod = 0.1f;
        float k_nh4 = 0.05f;
        float tempFactor = std::pow(1.035f, inletWater.getParameter(TEMP) - 20.0f);

        float BOD_in = inletWater.getParameter(BOD);
        float COD_in = inletWater.getParameter(COD);
        float NH4_in = inletWater.getParameter(NH4);

        float BOD_removal = BOD_in * (1 - std::exp(-k_bod * HRT * tempFactor));
        float COD_removal = COD_in * (1 - std::exp(-k_cod * HRT * tempFactor));
        float NH4_removal = NH4_in * (1 - std::exp(-k_nh4 * HRT * tempFactor));

        outletWater = inletWater;
        outletWater.updateParameter(BOD, BOD_in - BOD_removal);
        outletWater.updateParameter(COD, COD_in - COD_removal);
        outletWater.updateParameter(NH4, NH4_in - NH4_removal);
        outletWater.updateParameter(NO3, inletWater.getParameter(NO3) + NH4_removal * 0.9f);

        float DO_consumed = (BOD_removal + COD_removal + NH4_removal * 4.57f) * 1.5f;
        outletWater.updateParameter(DO, inletWater.getParameter(DO) - DO_consumed);
        if (outletWater.getParameter(DO) < 0) outletWater.updateParameter(DO, 0);
    }
};

//|........||Primary Sedimentation Tank: removes suspended solids and some BOD
class PrimarySedimentationTank : public Component {
public:
    PrimarySedimentationTank(const sf::Vector2f& pos) : Component(
        "Primary Sedimentation Tank",
        "Removes settleable solids and reduces BOD through sedimentation.",
        pos) {
        shape.setFillColor(sf::Color(139, 69, 19));
    }

    void simulate(float deltaTime) override {
        float removalEfficiencyTSS = 0.60f;
        float removalEfficiencyBOD = 0.35f;
        //pathogen
        float removalEfficiencyPathogen = 0.60f;
        outletWater = inletWater;
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) * (1 - removalEfficiencyTSS));
        outletWater.updateParameter(BOD, inletWater.getParameter(BOD) * (1 - removalEfficiencyBOD));
        outletWater.updateParameter(TURBIDITY, inletWater.getParameter(TURBIDITY) * 0.5f);
        outletWater.updateParameter(PATHOGENS, inletWater.getParameter(PATHOGENS) * (1 - removalEfficiencyPathogen));
    }
};

//|........||Aeration Tank: aerobic biological treatment to remove BOD and ammonium
class AerationTank : public Component {
public:
    AerationTank(const sf::Vector2f& pos) : Component(
        "Aeration Tank",
        "Promotes microbial degradation of organic matter under aerobic conditions.",
        pos) {
        shape.setFillColor(sf::Color(70, 130, 180));
    }

    void simulate(float deltaTime) override {
        float k_bod = 0.2f;
        float k_nh4 = 0.1f;
        float tempFactor = std::pow(1.035f, inletWater.getParameter(TEMP) - 20.0f);

        float BOD_in = inletWater.getParameter(BOD);
        float NH4_in = inletWater.getParameter(NH4);

        float BOD_removal = BOD_in * (1 - std::exp(-k_bod * HRT * tempFactor));
        float NH4_removal = NH4_in * (1 - std::exp(-k_nh4 * HRT * tempFactor));

        outletWater = inletWater;
        outletWater.updateParameter(BOD, BOD_in - BOD_removal);
        outletWater.updateParameter(NH4, NH4_in - NH4_removal);
        outletWater.updateParameter(NO3, inletWater.getParameter(NO3) + NH4_removal * 0.9f);

        float DO_consumed = (BOD_removal + NH4_removal * 4.57f) * 1.5f;
        outletWater.updateParameter(DO, inletWater.getParameter(DO) - DO_consumed);
        if (outletWater.getParameter(DO) < 0) outletWater.updateParameter(DO, 0);
    }
};

//|........||Secondary Clarifier: removes biomass generated in the aeration tank
class SecondaryClarifier : public Component {
public:
    SecondaryClarifier(const sf::Vector2f& pos) : Component(
        "Secondary Clarifier",
        "Settles out microbial biomass from the aeration tank.",
        pos) {
        shape.setFillColor(sf::Color(210, 180, 140));
    }

    void simulate(float deltaTime) override {
        float TSS_removal = inletWater.getParameter(TSS) * 0.85f;
        outletWater = inletWater;
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) - TSS_removal);
        outletWater.updateParameter(TURBIDITY, inletWater.getParameter(TURBIDITY) * 0.7f);
    }
};

//|........||Chlorine Disinfection Unit: eliminates pathogens using chlorine
class ChlorineDisinfectionUnit : public Component {
public:
    ChlorineDisinfectionUnit(const sf::Vector2f& pos) : Component(
        "Chlorine Disinfection Unit",
        "Uses chlorine to disinfect water, killing remaining pathogens.",
        pos) {
        shape.setFillColor(sf::Color(255, 215, 0));
    }

    void simulate(float deltaTime) override {
        float pathogen_removal = inletWater.getParameter(PATHOGENS) * 0.99999f;
        outletWater = inletWater;
        outletWater.updateParameter(PATHOGENS, inletWater.getParameter(PATHOGENS) - pathogen_removal);
        outletWater.updateParameter(RESIDUAL_CHLORINE, 0.7f); //|........||Add residual chlorine
        //Adds chlorides 46% more than the original value
        outletWater.updateParameter(CHLORIDES, inletWater.getParameter(CHLORIDES) * 1.46f);
    }
};

//|........||Nitrification Tank: converts ammonium to nitrate through nitrification
class NitrificationTank : public Component {
public:
    NitrificationTank(const sf::Vector2f& pos) : Component(
        "Nitrification Tank",
        "Biological process to convert ammonium to nitrate through nitrification.",
        pos) {
        shape.setFillColor(sf::Color(255, 165, 0));
    }

    void simulate(float deltaTime) override {
        float k_nh4 = 0.1f;
        float tempFactor = std::pow(1.035f, inletWater.getParameter(TEMP) - 20.0f);
        float alkalinityremoval = 0.5f;

        float NH4_in = inletWater.getParameter(NH4);
        float NO3_in = inletWater.getParameter(NO3);

        float NH4_removal = NH4_in * (1 - std::exp(-k_nh4 * HRT * tempFactor));
        float NO3_generated = NH4_removal * 0.9f;

        outletWater = inletWater;
        outletWater.updateParameter(NH4, NH4_in - NH4_removal);
        outletWater.updateParameter(NO3, NO3_in + NO3_generated);
        outletWater.updateParameter(ALKALINITY, inletWater.getParameter(ALKALINITY) - alkalinityremoval);
    }
};

//|........||UV Disinfection: uses ultraviolet light to eliminate pathogens without chemicals
class UVDisinfection : public Component {
public:
    UVDisinfection(const sf::Vector2f& pos) : Component(
        "UV Disinfection",
        "Utilizes UV radiation to inactivate pathogens without chemical additives.",
        pos) {
        shape.setFillColor(sf::Color(255, 255, 224));
    }

    void simulate(float deltaTime) override {
        float pathogen_removal = inletWater.getParameter(PATHOGENS) * 0.999f;
        outletWater = inletWater;
        outletWater.updateParameter(PATHOGENS, inletWater.getParameter(PATHOGENS) - pathogen_removal);
    }
};

//|........||Anaerobic Filter: removes organic matter through anaerobic processes
class AnaerobicFilter : public Component {
public:
    AnaerobicFilter(const sf::Vector2f& pos) : Component(
        "Anaerobic Filter",
        "Employs anaerobic bacteria to degrade organic pollutants.",
        pos) {
        shape.setFillColor(sf::Color(85, 107, 47));
    }

    void simulate(float deltaTime) override {
        float COD_in = inletWater.getParameter(COD);
        float COD_removal = COD_in * 0.65f;
        outletWater = inletWater;
        outletWater.updateParameter(COD, COD_in - COD_removal);
        //|........||Biogas production (e.g., methane)
    }
};

//|........||Coagulation and Flocculation: destabilizes particles for COD removal
class CoagulationFlocculation : public Component {
public:
    CoagulationFlocculation(const sf::Vector2f& pos) : Component(
        "Coagulation and Flocculation",
        "Destabilizes particles for subsequent removal of COD and TSS.",
        pos) {
        shape.setFillColor(sf::Color(128, 0, 128));
    }

    void simulate(float deltaTime) override {
        float COD_removal = inletWater.getParameter(COD) * 0.40f;
        float TSS_removal = inletWater.getParameter(TSS) * 0.60f;
        outletWater = inletWater;
        outletWater.updateParameter(COD, inletWater.getParameter(COD) - COD_removal);
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) - TSS_removal);
    }
};

//|........||Membrane Filtration: uses ultrafiltration to remove particles and pathogens and COD
class MembraneFiltration : public Component {
public:
    MembraneFiltration(const sf::Vector2f& pos) : Component(
        "Membrane Filtration",
        "Removes particles, pathogens, and COD through ultrafiltration membranes.",
        pos) {
        shape.setFillColor(sf::Color(0, 0, 255));
    }

    void simulate(float deltaTime) override {
        float COD_removal = inletWater.getParameter(COD) * 0.20f;
        float TSS_removal = inletWater.getParameter(TSS) * 0.45f;
        float pathogen_removal = inletWater.getParameter(PATHOGENS) * 0.9999f;
        outletWater = inletWater;
        outletWater.updateParameter(COD, inletWater.getParameter(COD) - COD_removal);
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) - TSS_removal);
        outletWater.updateParameter(PATHOGENS, inletWater.getParameter(PATHOGENS) - pathogen_removal);
    }
};

//|........||Chemical Oxidation: uses oxidizing agents to remove COD and color
class ChemicalOxidation : public Component {
public:
    ChemicalOxidation(const sf::Vector2f& pos) : Component(
        "Chemical Oxidation",
        "Applies strong oxidants to degrade organic pollutants and color.",
        pos) {
        shape.setFillColor(sf::Color(255, 0, 0));
    }

    void simulate(float deltaTime) override {
        float COD_removal = inletWater.getParameter(COD) * 0.70f;
        outletWater = inletWater;
        outletWater.updateParameter(COD, inletWater.getParameter(COD) - COD_removal);
        outletWater.updateParameter(TURBIDITY, inletWater.getParameter(TURBIDITY) * 0.8f);
    }
};

//|........||Active Sludge Process: aerobic biological treatment for BOD, COD and TSS removal
class ActiveSludgeProcess : public Component {
public:
    ActiveSludgeProcess(const sf::Vector2f& pos) : Component(
        "Active Sludge Process",
        "Biological treatment to remove BOD, COD, and TSS through aeration and sedimentation.",
        pos) {
        shape.setFillColor(sf::Color(70, 130, 180));
    }

    void simulate(float deltaTime) override {
        float k_bod = 0.2f;
        float k_nh4 = 0.1f;
        float tempFactor = std::pow(1.035f, inletWater.getParameter(TEMP) - 20.0f);

        float BOD_in = inletWater.getParameter(BOD);
        float NH4_in = inletWater.getParameter(NH4);

        float BOD_removal = BOD_in * (1 - std::exp(-k_bod * HRT * tempFactor));
        float NH4_removal = NH4_in * (1 - std::exp(-k_nh4 * HRT * tempFactor));

        outletWater = inletWater;
        outletWater.updateParameter(BOD, BOD_in - BOD_removal);
        outletWater.updateParameter(NH4, NH4_in - NH4_removal);
        outletWater.updateParameter(NO3, inletWater.getParameter(NO3) + NH4_removal * 0.9f);

        float DO_consumed = (BOD_removal + NH4_removal * 4.57f) * 1.5f;
        outletWater.updateParameter(DO, inletWater.getParameter(DO) - DO_consumed);
        if (outletWater.getParameter(DO) < 0) outletWater.updateParameter(DO, 0);
        //COD removal
        float COD_removal = inletWater.getParameter(COD) * 0.79f;
        outletWater.updateParameter(COD, inletWater.getParameter(COD) - COD_removal);
    }
};

//|........||Sludge Digester: stabilizes sludge through anaerobic digestion
class SludgeDigester : public Component {
public:
    SludgeDigester(const sf::Vector2f& pos) : Component(
        "Sludge Digester",
        "Reduces sludge volume and stabilizes organic content anaerobically.",
        pos) {
        shape.setFillColor(sf::Color(165, 42, 42));
    }

    void simulate(float deltaTime) override {
        float VSS_removal = inletWater.getParameter(TSS) * 0.55f;
        outletWater = inletWater;
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) - VSS_removal);
        //|........||Further pathogen reduction
    }
};

//|........||Oil and Grease Separator: removes oils and greases through flotation
class OilSeparator : public Component {
public:
    OilSeparator(const sf::Vector2f& pos) : Component(
        "Oil and Grease Separator",
        "Separates oils and greases from water by flotation mechanisms.",
        pos) {
        shape.setFillColor(sf::Color(255, 160, 122));
    }

    void simulate(float deltaTime) override {
        float oil_removal = inletWater.getParameter(OIL) * 0.90f;
        outletWater = inletWater;
        outletWater.updateParameter(OIL, inletWater.getParameter(OIL) - oil_removal);
        outletWater.updateParameter(TURBIDITY, inletWater.getParameter(TURBIDITY) * 0.9f);
    }
};

//|........||Phosphorus Removal Unit: chemical precipitation to remove phosphorus
class PhosphorusRemovalUnit : public Component {
public:
    PhosphorusRemovalUnit(const sf::Vector2f& pos) : Component(
        "Phosphorus Removal Unit",
        "Eliminates phosphorus via chemical precipitation methods.",
        pos) {
        shape.setFillColor(sf::Color(138, 43, 226));
    }

    void simulate(float deltaTime) override {
        float P_removal = inletWater.getParameter(P) * 0.75f;
        outletWater = inletWater;
        outletWater.updateParameter(P, inletWater.getParameter(P) - P_removal);
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) + P_removal * 2); //|........||Precipitate formation
    }
};

//|........||Drying Bed: removes moisture from sludge for final disposal
class DryingBed : public Component {
public:
    DryingBed(const sf::Vector2f& pos) : Component(
        "Drying Bed",
        "Allows for dewatering of sludge through evaporation and drainage.",
        pos) {
        shape.setFillColor(sf::Color(222, 184, 135));
    }

    void simulate(float deltaTime) override {
        float moisture_removal = inletWater.getParameter(TSS) * 0.95f;
        outletWater = inletWater;
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) - moisture_removal);
    }
};

//|........||Pump: increases fluid energy to move it through the system
class Pump : public Component {
public:
    Pump(const sf::Vector2f& pos) : Component(
        "Pump",
        "Boosts water pressure to facilitate flow through the treatment processes.",
        pos) {
        shape.setFillColor(sf::Color(105, 105, 105));
    }

    void simulate(float deltaTime) override {
        //|........||Pressure or head increase calculations
        outletWater = inletWater;
    }
};

//|........||Flow Meter: measures the flow rate in the system
class FlowMeter : public Component {
public:
    FlowMeter(const sf::Vector2f& pos) : Component(
        "Flow Meter",
        "Monitors the flow rate of water for system control and optimization.",
        pos) {
        shape.setFillColor(sf::Color(0, 191, 255));
    }

    void simulate(float deltaTime) override {
        //|........||Flow measurement logic
        outletWater = inletWater;
    }
};

//|........||Water Softener: removes hardness through ion exchange
class WaterSoftener : public Component {
public:
    WaterSoftener(const sf::Vector2f& pos) : Component(
        "Water Softener",
        "Reduces water hardness by exchanging calcium and magnesium ions for sodium ions.",
        pos) {
        shape.setFillColor(sf::Color(176, 196, 222));
    }

    void simulate(float deltaTime) override {
        float hardness_removal = inletWater.getParameter(HARDNESS) * 0.90f;
        outletWater = inletWater;
        outletWater.updateParameter(HARDNESS, inletWater.getParameter(HARDNESS) - hardness_removal);
        //|........||Increase of sodium in water
    }
};

//|........||Activated Carbon Filter: removes organic compounds and improves taste and odor
class ActivatedCarbonFilter : public Component {
public:
    ActivatedCarbonFilter(const sf::Vector2f& pos) : Component(
        "Activated Carbon Filter",
        "Adsorbs organic pollutants, enhancing taste and odor quality.",
        pos) {
        shape.setFillColor(sf::Color(47, 79, 79));
    }

    void simulate(float deltaTime) override {
        float COD_removal = inletWater.getParameter(COD) * 0.30f;
        outletWater = inletWater;
        outletWater.updateParameter(COD, inletWater.getParameter(COD) - COD_removal);
        //|........||Removal of volatile organic compounds
    }
};

//|........||Heat Exchanger: adjusts the temperature of the water
class HeatExchanger : public Component {
public:
    HeatExchanger(const sf::Vector2f& pos) : Component(
        "Heat Exchanger",
        "Regulates water temperature for optimal treatment conditions.",
        pos) {
        shape.setFillColor(sf::Color(250, 128, 114));
    }

    void simulate(float deltaTime) override {
        outletWater = inletWater;
        outletWater.updateParameter(TEMP, 25.0f); //|........||Sets temperature to 25°C
    }
};

//|........||Heavy Metals Removal Unit: removes heavy metals through adsorption or precipitation
class MetalsRemovalUnit : public Component {
public:
    MetalsRemovalUnit(const sf::Vector2f& pos) : Component(
        "Metals Removal Unit",
        "Eliminates heavy metals to prevent toxicity in the environment.",
        pos) {
        shape.setFillColor(sf::Color(112, 128, 144));
    }

    void simulate(float deltaTime) override {
        float metals_removal = inletWater.getParameter(METALS) * 0.85f;
        outletWater = inletWater;
        outletWater.updateParameter(METALS, inletWater.getParameter(METALS) - metals_removal);
        //|........||Handling of metal-laden sludge
    }
};

//|........||Membrane Filtration Unit: employs membranes to remove fine particles and microbes
class MembraneFiltrationUnit : public Component {
public:
    MembraneFiltrationUnit(const sf::Vector2f& pos) : Component(
        "Membrane Filtration Unit",
        "Uses microfiltration or ultrafiltration membranes for fine particle removal.",
        pos) {
        shape.setFillColor(sf::Color(72, 61, 139));
    }

    void simulate(float deltaTime) override {
        float pathogens_removal = inletWater.getParameter(PATHOGENS) * 0.9999f;
        float TSS_removal = inletWater.getParameter(TSS) * 0.99f;
        outletWater = inletWater;
        outletWater.updateParameter(PATHOGENS, inletWater.getParameter(PATHOGENS) - pathogens_removal);
        outletWater.updateParameter(TSS, inletWater.getParameter(TSS) - TSS_removal);
    }
};

//|........||Reverse Osmosis Unit: removes dissolved salts and small molecules
class ReverseOsmosisUnit : public Component {
public:
    ReverseOsmosisUnit(const sf::Vector2f& pos) : Component(
        "Reverse Osmosis Unit",
        "Employs semi-permeable membranes to desalinate and purify water.",
        pos) {
        shape.setFillColor(sf::Color(60, 179, 113));
    }

    void simulate(float deltaTime) override {
        float salinity_removal = inletWater.getParameter(SALINITY) * 0.95f;
        //Conductivity reduction
        float EC_removal = inletWater.getParameter(EC) * 0.95f;
        outletWater = inletWater;
        outletWater.updateParameter(SALINITY, inletWater.getParameter(SALINITY) - salinity_removal);
        outletWater.updateParameter(EC, inletWater.getParameter(EC) * 0.05f);
    }
};

//|........||Connections between components
class Connection {
public:
    Component* from;
    Component* to;
    std::vector<sf::CircleShape> flowParticles;
    float particleSpawnTime = 0.0f;
    float diameter = 10.0f; //|........||Pipe diameter in cm

    Connection(Component* f, Component* t) : from(f), to(t) {}

    void update(float deltaTime) {
        particleSpawnTime += deltaTime;
        if (particleSpawnTime >= 0.02f) {
            particleSpawnTime = 0.0f;
            sf::CircleShape particle(3.0f);
            float bod = from->outletWater.getParameter(BOD);
            sf::Color color = sf::Color::Cyan;
            if (bod > 200) color = sf::Color(139, 0, 0);
            else if (bod > 100) color = sf::Color(255, 69, 0);
            else if (bod > 50) color = sf::Color(255, 165, 0);
            else if (bod > 10) color = sf::Color(255, 255, 0);
            else color = sf::Color(0, 255, 0);

            particle.setFillColor(color);
            particle.setPosition(from->position.x + from->width, from->position.y + from->height / 2);
            flowParticles.push_back(particle);
        }

        for (auto& particle : flowParticles) {
            sf::Vector2f direction = to->position - from->position;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction /= length;
            particle.move(direction * 150.0f * deltaTime);
        }

        flowParticles.erase(std::remove_if(flowParticles.begin(), flowParticles.end(),
            [this](sf::CircleShape& p) {
                return p.getPosition().x >= to->position.x;
            }), flowParticles.end());
    }

    void draw(sf::RenderWindow& window) {
        sf::RectangleShape pipe;
        pipe.setPosition(from->position.x + from->width, from->position.y + from->height / 2 - diameter / 2);
        pipe.setSize(sf::Vector2f(to->position.x - (from->position.x + from->width), diameter));
        pipe.setFillColor(sf::Color(70, 70, 70));
        window.draw(pipe);

        for (auto& particle : flowParticles) {
            window.draw(particle);
        }
    }
};

//|........||Estructura para almacenar datos históricos
struct ParameterHistory {
    std::deque<float> values;
    size_t maxSize;
    ParameterHistory(size_t size = 100) : maxSize(size) {}
    void addValue(float val) {
        if (values.size() >= maxSize) values.pop_front();
        values.push_back(val);
    }
};

//|........||Mapa para almacenar el historial de cada parámetro
std::map<WaterParameter, ParameterHistory> parameterHistories;

//|........||Actualizar los historiales después de cada simulación
void updateHistories(const std::vector<Component*>& components) {
    for (const auto& comp : components) {
        for (const auto& param : comp->outletWater.parameters) {
            parameterHistories[param.first].addValue(param.second);
        }
    }
}

//|........||Función para dibujar gráficos
void drawGraphs() {
    ImGui::Begin("Gráficos de Parámetros");

    for (auto& [param, history] : parameterHistories) {
        std::string label = parameterToString(param);
        if (ImGui::CollapsingHeader(label.c_str())) {
            if (!history.values.empty()) {
                std::vector<float> plotData(history.values.begin(), history.values.end());
                ImGui::PlotLines(" ", plotData.data(), plotData.size(), 0, NULL, 0.0f, 1000.0f, ImVec2(0, 100));
            }
        }
    }

    ImGui::End();
}

void setImGuiStyle() {
    ImGui::StyleColorsDark(); //|........||Usar colores oscuros como base

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;

    //|........||Personalizar colores
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    //|........||Puedes personalizar más colores según prefieras
}

//|........||Main function
int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "WWTP Simulator @FECORO");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    setImGuiStyle();

    std::vector<Component*> components;
    std::vector<Connection*> connections;

    Inlet* inlet = new Inlet(sf::Vector2f(50, 440));
    Outlet* outlet = new Outlet(sf::Vector2f(1650, 440));
    components.push_back(inlet);
    components.push_back(outlet);

    std::string newComponentType = "Primary Sedimentation Tank";
    std::vector<std::string> componentTypes = {
        "Primary Sedimentation Tank",
        "Primary Clarifier",
        "Aeration Tank",
        "Secondary Clarifier",
        "Chlorine Disinfection Unit",
        "UV Disinfection",
        "Anaerobic Filter",
        "Sludge Digester",
        "Oil and Grease Separator",
        "Phosphorus Removal Unit",
        "Drying Bed",
        "Pump",
        "Flow Meter",
        "Water Softener",
        "Activated Carbon Filter",
        "Heat Exchanger",
        "Metals Removal Unit",
        "Membrane Filtration Unit",
        "Reverse Osmosis Unit",
        "Coagulation and Flocculation",
        "Membrane Filtration",
        "Chemical Oxidation",
        "Active Sludge Process",
        "Nitrification Tank",
        "Biofilter",
        "Filtration",
        "Membrane Bioreactor",
        "Ozone Disinfection",
        "Anaerobic-Aerobic Treatment",
        "Electrocoagulation Unit",
    };

    bool isSimulating = false;
    float simulationSpeed = 1.0f;
    bool showInletOutletWindow = true;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = deltaClock.restart().asSeconds() * simulationSpeed;

        if (isSimulating) {
            for (size_t i = 0; i < components.size(); ++i) {
                if (i == 0) continue;
                components[i]->simulate(deltaTime);
            }

            for (size_t i = 1; i < components.size(); ++i) {
                components[i]->inletWater = components[i - 1]->outletWater;
            }

            //|........||Actualizar historiales
            updateHistories(components);
        }

        for (auto& comp : components) {
            comp->update(deltaTime);
        }
        for (auto& conn : connections) {
            conn->update(deltaTime);
        }

        ImGui::SFML::Update(window, sf::seconds(SIMULATION_TIME_STEP));

        //|........||Dibujar gráficos
        drawGraphs();

        //|........||Ventana adicional de IMGUI para Inlet y Outlet
        if (showInletOutletWindow) {
            ImGui::Begin("Parámetros de Inlet y Outlet", &showInletOutletWindow);

            //|........||Parámetros de Inlet
            ImGui::Text("Inlet:");
            for (const auto& param : inlet->outletWater.parameters) {
                float value = param.second;
                if (ImGui::InputFloat(("Inlet " + parameterToString(param.first)).c_str(), &value)) {
                    inlet->outletWater.updateParameter(param.first, value);
                }
            }
            ImGui::Separator();

            //|........||Parámetros de Outlet
            ImGui::Text("Outlet:");
            for (const auto& param : outlet->inletWater.parameters) {
                float value = param.second;
                ImGui::Text("%s: %.2f", ("Outlet " + parameterToString(param.first)).c_str(), value);
            }

            ImGui::End();
        }

        ImGui::Begin("Control Panel");

        if (ImGui::Button("Start Simulation")) {
            isSimulating = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop Simulation")) {
            isSimulating = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            for (auto& comp : components) {
                if (comp != inlet && comp != outlet) {
                    delete comp;
                }
            }
            components.clear();
            components.push_back(inlet);
            components.push_back(outlet);
            connections.clear();
        }
        if (ImGui::Button("Load Default Example")) {
            //|........||Reset current components
            for (auto& comp : components) {
                if (comp != inlet && comp != outlet) {
                    delete comp;
                }
            }
            components.clear();
            components.push_back(inlet);
            components.push_back(outlet);
            connections.clear();

            //|........||Add typical components
            std::vector<std::string> defaultComponents = {
                "Primary Clarifier",
                "Primary Sedimentation Tank",
                "Aeration Tank",
                "Active Sludge Process",
                "Nitrification Unit",
                "Secondary Clarifier",
                "Chlorine Disinfection Unit",
                "Filtration"
            };
            for (const auto& type : defaultComponents) {
                sf::Vector2f position(components.back()->position.x - 150 * (components.size() - 1), 440);
                Component* comp = nullptr;
                if (type == "Primary Clarifier") comp = new PrimaryClarifier(position);
                else if (type == "Primary Sedimentation Tank") comp = new PrimarySedimentationTank(position);
                else if (type == "Aeration Tank") comp = new AerationTank(position);
                else if (type == "Active Sludge Process") comp = new ActiveSludgeProcess(position);
                else if (type == "Nitrification Unit") comp = new NitrificationTank(position);
                else if (type == "Secondary Clarifier") comp = new SecondaryClarifier(position);
                else if (type == "Chlorine Disinfection Unit") comp = new ChlorineDisinfectionUnit(position);
                else if (type == "Filtration") comp = new Filtration(position);
                components.insert(components.end() - 1, comp);
            }
            //|........||Adjust positions
            for (size_t i = 1; i < components.size(); ++i) {
                components[i]->position.x = components[i - 1]->position.x + 150;
                components[i]->shape.setPosition(components[i]->position);
            }
            //|........||Create connections
            for (size_t i = 0; i < components.size() - 1; ++i) {
                connections.push_back(new Connection(components[i], components[i + 1]));
            }
        }

        ImGui::SliderFloat("Simulation Speed", &simulationSpeed, 0.1f, 5.0f);

        ImGui::Separator();
        ImGui::Text("Add Component:");
        if (ImGui::BeginCombo("Type", newComponentType.c_str())) {
            for (const auto& type : componentTypes) {
                bool isSelected = (newComponentType == type);
                if (ImGui::Selectable(type.c_str(), isSelected)) {
                    newComponentType = type;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if (ImGui::Button("Add")) {
            sf::Vector2f position(components.back()->position.x - 150 * (components.size() - 1), 440);
            Component* comp = nullptr;
            if (newComponentType == "Primary Sedimentation Tank") comp = new PrimarySedimentationTank(position);
            else if (newComponentType == "Primary Clarifier") comp = new PrimaryClarifier(position);
            else if (newComponentType == "Aeration Tank") comp = new AerationTank(position);
            else if (newComponentType == "Secondary Clarifier") comp = new SecondaryClarifier(position);
            else if (newComponentType == "Chlorine Disinfection Unit") comp = new ChlorineDisinfectionUnit(position);
            else if (newComponentType == "UV Disinfection") comp = new UVDisinfection(position);
            else if (newComponentType == "Anaerobic Filter") comp = new AnaerobicFilter(position);
            else if (newComponentType == "Sludge Digester") comp = new SludgeDigester(position);
            else if (newComponentType == "Oil and Grease Separator") comp = new OilSeparator(position);
            else if (newComponentType == "Phosphorus Removal Unit") comp = new PhosphorusRemovalUnit(position);
            else if (newComponentType == "Drying Bed") comp = new DryingBed(position);
            else if (newComponentType == "Pump") comp = new Pump(position);
            else if (newComponentType == "Flow Meter") comp = new FlowMeter(position);
            else if (newComponentType == "Water Softener") comp = new WaterSoftener(position);
            else if (newComponentType == "Activated Carbon Filter") comp = new ActivatedCarbonFilter(position);
            else if (newComponentType == "Heat Exchanger") comp = new HeatExchanger(position);
            else if (newComponentType == "Metals Removal Unit") comp = new MetalsRemovalUnit(position);
            else if (newComponentType == "Membrane Filtration Unit") comp = new MembraneFiltrationUnit(position);
            else if (newComponentType == "Reverse Osmosis Unit") comp = new ReverseOsmosisUnit(position);
            else if (newComponentType == "Coagulation and Flocculation") comp = new CoagulationFlocculation(position);
            else if (newComponentType == "Membrane Filtration") comp = new MembraneFiltration(position);
            else if (newComponentType == "Chemical Oxidation") comp = new ChemicalOxidation(position);
            else if (newComponentType == "Active Sludge Process") comp = new ActiveSludgeProcess(position);
            else if (newComponentType == "Nitrification Unit") comp = new NitrificationTank(position);
            else if (newComponentType == "Biofilter Unit") comp = new Biofilter(position);
            else if (newComponentType == "Filtration") comp = new Filtration(position);
            else if (newComponentType == "Membrane Bioreactor") comp = new MBR(position);
            else if (newComponentType == "Ozonation Unit") comp = new OzoneDisinfection(position);
            else if (newComponentType == "Anaerobic-Aerobic Treatment") comp = new AnaerobicAerobicFilter(position);
            else if (newComponentType == "Electrocoagulation Unit") comp = new ElectrocoagulationUnit(position);

            if (comp) {
                components.insert(components.end() - 1, comp);
                for (size_t i = 1; i < components.size(); ++i) {
                    components[i]->position.x = components[i - 1]->position.x + 150;
                    components[i]->shape.setPosition(components[i]->position);
                }

                connections.clear();
                for (size_t i = 0; i < components.size() - 1; ++i) {
                    connections.push_back(new Connection(components[i], components[i + 1]));
                }
            }
        }

        ImGui::Separator();
        ImGui::Text("Components:");
        for (size_t i = 0; i < components.size(); ++i) {
            Component* comp = components[i];
            ImGui::PushID((int)i);
            if (ImGui::CollapsingHeader(comp->name.c_str())) {
                ImGui::TextWrapped("%s", comp->description.c_str());
                if (i == 0) {
                    ImGui::Text("Input Parameters:");
                    for (auto& param : comp->outletWater.parameters) {
                        float value = param.second;
                        if (ImGui::InputFloat(parameterToString(param.first).c_str(), &value)) {
                            comp->outletWater.updateParameter(param.first, value);
                        }
                    }
                } else if (i == components.size() - 1) {
                    ImGui::Text("Output Parameters:");
                    for (auto& param : comp->inletWater.parameters) {
                        float value = param.second;
                        ImGui::Text("%s: %.2f", parameterToString(param.first).c_str(), value);
                    }
                } else {
                    ImGui::InputFloat("Volume (m³)", &comp->volume);
                    ImGui::InputFloat("Flow Rate (m³/day)", &comp->flowRate);
                    ImGui::InputFloat("HRT (hrs)", &comp->HRT);
                    ImGui::InputFloat("SRT (days)", &comp->SRT);
                    ImGui::InputFloat("Temperature (°C)", &comp->temperature);
                    if (ImGui::Button("Remove")) {
                        delete comp;
                        components.erase(components.begin() + i);
                        connections.clear();
                        for (size_t j = 0; j < components.size() - 1; ++j) {
                            connections.push_back(new Connection(components[j], components[j + 1]));
                        }
                        break;
                    }
                    ImGui::Text("Water Parameters:");
                    for (auto& param : comp->outletWater.parameters) {
                        float value_in = comp->inletWater.getParameter(param.first);
                        float value_out = comp->outletWater.getParameter(param.first);
                        ImGui::Text("%s - Inlet: %.2f, Outlet: %.2f", parameterToString(param.first).c_str(), value_in, value_out);
                    }
                    ImGui::Separator();
                    ImGui::Text("Removal Efficiencies:");
                    for (auto& param : comp->outletWater.parameters) {
                        float efficiency = comp->removalEfficiencies[param.first];
                        if (ImGui::SliderFloat(("Efficiency " + parameterToString(param.first)).c_str(), &efficiency, -1.0f, 1.0f)) {
                            comp->addRemovalEfficiency(param.first, efficiency);
                        }
                        if (ImGui::Button(("Remove " + parameterToString(param.first)).c_str())) {
                            comp->removeRemovalEfficiency(param.first);
                        }
                    }
                }
            }
            ImGui::PopID();
        }

        ImGui::End();

        window.clear(sf::Color::White);

        for (auto& conn : connections) {
            conn->draw(window);
        }

        for (auto& comp : components) {
            comp->draw(window);
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    for (auto& comp : components) {
        delete comp;
    }
    for (auto& conn : connections) {
        delete conn;
    }

    ImGui::SFML::Shutdown();
    return 0;
}

//|........ End of WWTPSIM.cpp ........|
// @Copyright, all rights reserved.
