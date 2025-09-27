#pragma once

#include <string>
#include <map>
#include <vector>

namespace stl_to_eznec {

struct MaterialProperties {
    std::string name;
    double conductivity;  // S/m
    double relativePermittivity;
    std::string description;
    
    MaterialProperties() : name(""), conductivity(0.0), relativePermittivity(1.0), description("") {}
    
    MaterialProperties(const std::string& name, double conductivity, double relativePermittivity, const std::string& description = "")
        : name(name), conductivity(conductivity), relativePermittivity(relativePermittivity), description(description) {}
};

class MaterialDatabase {
public:
    MaterialDatabase();
    
    // Get all available materials
    const std::vector<MaterialProperties>& getAllMaterials() const { return materials_; }
    
    // Get material by name
    const MaterialProperties* getMaterial(const std::string& name) const;
    
    // Get material by index
    const MaterialProperties* getMaterial(int index) const;
    
    // Get material count
    size_t getMaterialCount() const { return materials_.size(); }
    
    // Print material list for user selection
    void printMaterialList() const;
    
    // Validate material selection
    bool isValidMaterial(const std::string& name) const;
    bool isValidMaterialIndex(int index) const;

private:
    std::vector<MaterialProperties> materials_;
    std::map<std::string, int> materialIndex_;
    
    void initializeMaterials();
};

// Water properties for marine applications
struct WaterProperties {
    double conductivity;  // S/m
    double relativePermittivity;
    std::string type;
    
    WaterProperties(double conductivity, double relativePermittivity, const std::string& type)
        : conductivity(conductivity), relativePermittivity(relativePermittivity), type(type) {}
};

class WaterDatabase {
public:
    WaterDatabase();
    
    const WaterProperties* getFreshWater() const { return &freshWater_; }
    const WaterProperties* getSaltWater() const { return &saltWater_; }
    
    void printWaterTypes() const;

private:
    WaterProperties freshWater_;
    WaterProperties saltWater_;
};

} // namespace stl_to_eznec
