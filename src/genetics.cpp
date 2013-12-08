#include "genetics.h"

Genetics::Genetics()
  : crossoverRate(CFG->readInt("CrossoverRate")), mutationGaussDeviation(CFG->readFloat("MutationGaussDeviation")) {}

void Genetics::evolve(EntityManager &manager) {
	AnimalData tmp;
	
	animalsData.clear();
	
	for (int i = manager.getAnimalsIndex(); i < manager.getAnimalsIndex() + manager.getSpeciesNumber(); i++) {	
		parents.clear();
		
		for (unsigned int j = 0; j < manager.getEntities()[i].tab.size(); j++) {
			Animal *animal = (Animal*) manager.getEntities()[i].tab[j];
			
			tmp.score = animal->getScore();
			tmp.DNA = animal->getDNA();
			tmp.cumulatedScore = 0;	

			parents.push_back(tmp);
		}
		
		// tri descendant du tableau parents par score
		sortData(parents, 0);
		
		// Roulette pour les parents sélectionnés
		roulette();
	
		// maj de l'ADN des animaux de l'espece
		for (unsigned int j = 0; j < children.size(); j++) {
			((Animal*) manager.getEntities()[i].tab[j])->init(children[j].DNA);
		}
	}
}

// Algorithme génétique utilisant roulette wheel, crossover et mutation
// TODO : Une fonction de 160 lignes c'est mal
void Genetics::roulette() {
	children.clear();
	
	// Récupération du nombre d'animaux
	unsigned int parentsNumber = parents.size();
	
	// Calcul du score cumulé de chaque animal :
	// parents[i].cumulatedScore = somme(k = 0 to i)(parents[k].score)
	// Si bien que le score du dernier parent = score total = cumulated à la fin de la boucle
	unsigned int cumulated = 0;
	for (unsigned int i = 0; i < parentsNumber; i++) {
		cumulated += parents[i].score + 1;
		
		parents[i].cumulatedScore = cumulated;
	}
	
	// Tant qu'on a pas créé autant d'enfants qu'il y avait de parents, on continue à en créer
	while (children.size() < parentsNumber) {
		unsigned int r1, r2, p1, p2;
		AnimalData tmp1, tmp2;
		
		// A moins que tous nos animaux soient VRAIMENT mauvais, on peut faire une roulette wheel
		if (cumulated != 0) {
			std::uniform_int_distribution<int> prob(0, cumulated);
			
			// Selection d'un premier parent (d'indice p1)
			// Le système roulette wheel permet de favoriser la selection de parents ayant un bon score tout en laissant une grande part d'aléatoire
			r1 = prob(generator);
			p1 = 0;
			while (p1 != (parentsNumber - 1) && r1 > parents[p1].cumulatedScore)
				p1++;
			
			// On fait la même sélection pour le 2e parent en s'assurant qu'on ne tombe pas 2x sur le même
			do {	
				r2 = prob(generator);
				p2 = 0;
			
				while (p2 != (parentsNumber - 1) && r2 > parents[p2].cumulatedScore)
					p2++;
			} while (p1 == p2);
		} else {
			// Sinon, selection random des parents
			std::uniform_int_distribution<int> prob(0, parentsNumber - 1);
			p1 = prob(generator);
			while ((p2 = prob(generator)) == p1);
		}
		
		std::uniform_int_distribution<int> randRate(0, 100);
		std::uniform_int_distribution<int> randomParent(0, 1);
		tmp1.DNA.clear();
		tmp2.DNA.clear();
		// On n'applique l'algorithme de dans crossoverRate% des cas, sinon on copie simplement l'ADN des parents dans les enfants
		if (randRate(generator) >= crossoverRate) {
			// Création de l'ADN de l'enfant en piochant aléatoirement dans l'ADN de l'un ou l'autre des parents
			// On utilise un random uniforme, donc normalement on devrait avoir 50% d'ADN de chaque
			
			// On génère deux enfants si possible
			if (parentsNumber - children.size() >= 2) {
				for (unsigned int j = 0; j < parents[0].DNA.size(); j++) {
					if (randomParent(generator) == 0) {
						tmp1.DNA.push_back(parents[p1].DNA[j]);
						tmp2.DNA.push_back(parents[p2].DNA[j]);
					} else {
						tmp1.DNA.push_back(parents[p2].DNA[j]);
						tmp2.DNA.push_back(parents[p1].DNA[j]);
					}
				}
				
				// On mute ensuite l'ADN des nouveaux nés
				mutation(tmp1.DNA);
				mutation(tmp2.DNA);
				
				children.push_back(tmp1);
				children.push_back(tmp2);
				
			// Sinon on en crée qu'un seul
			} else {
				for (unsigned int j = 0; j < parents[0].DNA.size(); j++) {
					if (randomParent(generator) == 0) {
						tmp1.DNA.push_back(parents[p1].DNA[j]);
					} else {
						tmp1.DNA.push_back(parents[p2].DNA[j]);
					}
				}
				
				// On mute ensuite l'ADN du nouveau né
				mutation(tmp1.DNA);
				
				children.push_back(tmp1);
			}
		} else {
			if (parentsNumber - children.size() >= 2) {
				tmp1.DNA = parents[p1].DNA;
				tmp2.DNA = parents[p2].DNA;
				
				// On mute ensuite l'ADN des nouveaux nés
				mutation(tmp1.DNA);
				mutation(tmp2.DNA);
				
				children.push_back(tmp1);
				children.push_back(tmp2);
			} else {
				if (randomParent(generator) == 0) {
					tmp1.DNA = parents[p1].DNA;
				} else {
					tmp1.DNA = parents[p2].DNA;
				}
				
				// On mute ensuite l'ADN du nouveau né
				mutation(tmp1.DNA);
				
				children.push_back(tmp1);
			}
		}
	}
}

void Genetics::mutation(std::vector<float> &DNA) {
	std::uniform_int_distribution<int> prob(0, DNA.size());
	for (unsigned int i = 0; i < DNA.size(); i++) {
		if (prob(generator) == 0) {
			float tmpf = DNA[i];
			std::normal_distribution<float> gauss(DNA[i], mutationGaussDeviation);
			
			DNA[i] = gauss(generator);
			
			tmpf = fabs(tmpf - DNA[i]);
		}
	}
}

void Genetics::sortData(std::vector<AnimalData> &data, const int asc) {
	AnimalData tmp;
	bool sorted = false;
	int k = 1;
	
	while (!sorted) {
		sorted = true;
		
		for (unsigned int i = 0; i < data.size() - k; i++) {
			if ((asc && (data[i].score > data[i+1].score)) ||
				(!asc && (data[i].score < data[i+1].score))) {	
				tmp = data[i];
				data[i] = data[i+1];
				data[i+1] = tmp;
				
				sorted = false;
			}
		}
		k++;
	}
}

