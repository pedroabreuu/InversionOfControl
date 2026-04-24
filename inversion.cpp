#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Subject;

// interface observer
class Observer {
public:
    virtual ~Observer() = default;
    /*
    O método update() funciona como o callback do observer.
    cada classe que quiser observar um Subject deve implementar esse método
    depois, o Subject pode chamar update() automaticamente quando seu estado mudar
    sem conhecer a classe concreta do observador.
    */
    virtual void update(Subject* subject) = 0;
};

class Subject {
private:
    std::vector<Observer*> observers;
public:
    virtual ~Subject() = default;
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }
    void removeObserver(Observer* observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
    void notifyObservers() {
        /*
        O Subject percorre todos os observers cadastrados e chama update(this)
        ele não sabe qual classe concreta está sendo chamada
        sabe apenas que todos seguem a interface do observer
        */
        for (Observer* observer : observers) {
            observer->update(this);
        }
    }
};

/*
A inversão de controle acontece no PCD, pois o Sujeito (PCD) é quem controla quando deve notificar os observers
ou seja, a inversão de controle acontece "naturalmente" ao utilizar o padrão do observer. Quando um PCD é atualizado, o próprio PCD decide
chamar notifyObservers(), notificando as instituições cadastradas como observers
Com isso, o controle da notificação fica no Subject/PCD e não na main ou no código cliente chamando update() manualmente
*/

class PCD: public Subject {
private:
    int id;
    double temperatura;
    double umidade;
    double pressao;
    double ph;
public:
    PCD(): id(0), temperatura(0.0), umidade(0.0), pressao(0.0), ph(0.0) {}
    void setId(int id) { this->id = id; }
    int getId() const { return id; }
    double getTemperatura() const { return temperatura; }
    double getUmidade() const { return umidade; }
    double getPressao() const { return pressao; }
    double getPh() const { return ph; }
    void setTemperatura(double temperatura) {
        this->temperatura = temperatura;
        notifyObservers();
    }
    void setUmidade(double umidade) {
        this->umidade = umidade;
        notifyObservers();
    }
    void setPressao(double pressao) {
        this->pressao = pressao;
        notifyObservers();
    }
    void setPh(double ph) {
        this->ph = ph;
        notifyObservers();
    }
};

//implementa observer
class ObserverPCD: public Observer {
private:
    std::string nome;
public:
    ObserverPCD() : nome("") {}
    void setNome(const std::string& nome) { this->nome = nome; }
    std::string getNome() const { return nome; }
    void update(Subject* subject) override {
        PCD* pcd = dynamic_cast<PCD*>(subject);
        if (!pcd) return;
        std::cout << nome << ": Houve atualizacao no PCD " << pcd->getId() << ".\n";
        std::cout << "Temperatura: " << pcd->getTemperatura() << "\n";
        std::cout << "Umidade: " << pcd->getUmidade() << "\n";
        std::cout << "Pressao: " << pcd->getPressao() << "\n";
        std::cout << "pH: " << pcd->getPh() << "\n\n";
    }
};

int main() {
    PCD rio1; 
    rio1.setId(1);

    PCD rio2; 
    rio2.setId(2);

    PCD rio3; 
    rio3.setId(3);

    ObserverPCD ufrgs; ufrgs.setNome("ufrgs");
    ObserverPCD pucrs; pucrs.setNome("PUC-RS");
    ObserverPCD pelotinhas; pelotinhas.setNome("Pelotas Business School");

    rio1.addObserver(&ufrgs);
    rio1.addObserver(&pucrs);
    rio2.addObserver(&ufrgs);
    rio2.addObserver(&pelotinhas);
    rio3.addObserver(&pucrs);
    rio3.addObserver(&pelotinhas);

    rio1.setTemperatura(25.0);
    rio2.setUmidade(80.0);
    rio3.setPressao(1013.0);
    return 0;
}