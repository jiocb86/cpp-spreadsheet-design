#pragma once

#include "common.h"
#include "formula.h"

class Sheet;

class Cell : public CellInterface {
public:
    Cell(Sheet& sheet);
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;

private:
    class Impl {
    public:
        virtual Value GetValue() const = 0;
        virtual std::string GetText() const = 0;
        virtual std::vector<Position> GetReferencedCells() const {
            return {};
        }
        virtual bool HasCache() const {
            return true;
        }
        virtual void InvalidateCache() {}        
        virtual ~Impl() = default;
    }; 
    class EmptyImpl;    
    class TextImpl;    
    class FormulaImpl;
    
    bool CheckCyclicDependency(const Impl& new_impl) const; // Проверка цикличности
    void UpdateDependencies(bool force = false);            // Обновление зависимостей    
    
    std::unique_ptr<Impl> impl_;                            // Указатель на реализацию    
    Sheet& sheet_;                                          // Ссылка на таблицу
    std::unordered_set<Cell*> dependent_сells_;             // Зависимые ячейки
    std::unordered_set<Cell*> referenced_сells_;            // Связанные ячейки
};