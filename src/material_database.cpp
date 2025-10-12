#include "material_database.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace stl_to_eznec {

MaterialDatabase::MaterialDatabase() {
    initializeMaterials();
}

void MaterialDatabase::initializeMaterials() {
    materials_.clear();
    materialIndex_.clear();
    
    // Vehicle materials
    materials_.emplace_back("Aluminum", 1.5e7, 1.0, "Aluminum alloys (6061, 2024, etc.)");
    materials_.emplace_back("Mild Steel", 7.0e6, 1.0, "Mild steel (car bodies, ship hulls)");
    materials_.emplace_back("Stainless Steel", 1.2e6, 1.0, "Stainless steel (304, 316)");
    materials_.emplace_back("Galvanized Steel", 4.0e6, 1.0, "Galvanized steel");
    materials_.emplace_back("Spring Steel", 3.0e6, 1.0, "High carbon spring steel");
    
    // Building materials
    materials_.emplace_back("Concrete", 0.5, 8.0, "Concrete building walls");
    
    // Create index map
    for (size_t i = 0; i < materials_.size(); ++i) {
        materialIndex_[materials_[i].name] = static_cast<int>(i);
    }
}

const MaterialProperties* MaterialDatabase::getMaterial(const std::string& name) const {
    auto it = materialIndex_.find(name);
    if (it != materialIndex_.end()) {
        return &materials_[it->second];
    }
    return nullptr;
}

const MaterialProperties* MaterialDatabase::getMaterial(int index) const {
    if (index >= 0 && index < static_cast<int>(materials_.size())) {
        return &materials_[index];
    }
    return nullptr;
}

void MaterialDatabase::printMaterialList() const {
    std::cout << "\n=== Material Selection ===\n";
    std::cout << "Select the material type for your vehicle:\n\n";
    
    for (size_t i = 0; i < materials_.size(); ++i) {
        const auto& material = materials_[i];
        std::cout << std::setw(2) << (i + 1) << ". " << material.name;
        std::cout << " (σ = " << std::scientific << std::setprecision(1) << material.conductivity;
        std::cout << " S/m, εᵣ = " << std::fixed << std::setprecision(1) << material.relativePermittivity << ")";
        std::cout << "\n     " << material.description << "\n\n";
    }
    
    std::cout << "These values are needed to set the material's electric properties correctly\n";
    std::cout << "for accurate electromagnetic simulation.\n\n";
}

bool MaterialDatabase::isValidMaterial(const std::string& name) const {
    return materialIndex_.find(name) != materialIndex_.end();
}

bool MaterialDatabase::isValidMaterialIndex(int index) const {
    return index >= 1 && index <= static_cast<int>(materials_.size());
}

WaterDatabase::WaterDatabase() 
    : freshWater_(0.001, 81.0, "Fresh Water")
    , saltWater_(4.5, 81.0, "Salt Water") {
}

void WaterDatabase::printWaterTypes() const {
    std::cout << "\n=== Water Type Selection ===\n";
    std::cout << "Select the water type for your marine vehicle:\n\n";
    
    std::cout << "1. Fresh Water (rivers, lakes)\n";
    std::cout << "   σ = " << std::scientific << std::setprecision(1) << freshWater_.conductivity;
    std::cout << " S/m, εᵣ = " << std::fixed << std::setprecision(1) << freshWater_.relativePermittivity << "\n\n";
    
    std::cout << "2. Salt Water (ocean)\n";
    std::cout << "   σ = " << std::scientific << std::setprecision(1) << saltWater_.conductivity;
    std::cout << " S/m, εᵣ = " << std::fixed << std::setprecision(1) << saltWater_.relativePermittivity << "\n\n";
}

} // namespace stl_to_eznec


