#include <set>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <set>
#include <iostream>
#include <sstream>

#include <json.hpp>

#include "entity.hpp"
#include "archetype.hpp"
#include "component.hpp"
#include "reflection.hpp"

int Archetype::allocateItem(EntityID entityID, bool initialize) {
	if (tables.empty() || tables.back()->count > 63) tables.push_back(new Table(this));
	Table* table = tables.back();
	if (initialize) for (int c = 1; c < 512; c++) if (mask[c]) ComponentConstructors[c](getPointer(c, getNextIndex()));
	for (int c = 0; c < 512; c++) if (mask[c]) tables[getNextIndex() / 64]->pointerGrid[componentIndex[c]][getNextIndex() % 64] = getPointer(c, getNextIndex());
	EntityID* id = (EntityID*)getPointer(0, getNextIndex());
	*id = { entityID };
	this->updateEntityStorage(*id, { this, getNextIndex() });
	auto idx = getNextIndex();
	table->count++;
	size++;
	return idx;
}

void Archetype::deallocateItem(int index, bool destruct) {
	Table* table = tables.back();
	for (int c = 0; c < 512; c++) if (mask[c])
		if (destruct)
			ComponentCopyScorchers[c](getPointer(c, getLastIndex()), getPointer(c, index));
		else
			ComponentCopiers[c](getPointer(c, getLastIndex()), getPointer(c, index));
	table->count--;
	for_each(table->pointerGrid.begin(), table->pointerGrid.end(), [index](auto& v) {
		v[index % 64] = nullptr;
		});
	if (table->count < 1) tables.pop_back();
	size--;
}

void Archetype::updateEntityStorage(EntityID entityID, Spot spot) {
	assert(es->isAlive(entityID));
	(*es)[entityID.index].archetype = spot.archetype;
	(*es)[entityID.index].archetypeIndex = spot.index;
}

Archetype::Archetype(World* owner, EntityStore* es, ComponentMask _mask, int level) : es(es) {
	_mask[0] = true;
	mask = _mask;
	setupOffsetMap();
	auto parentSpot = 0;
	for (int i = 0; i < 512; i++) {
		if (mask[i]) { components.push_back(i); componentIndex[i] = parentSpot++; }
		else { componentIndex[i] = -1; };
	}
}

int Archetype::getLastIndex() {
	return size - 1;
}

int Archetype::getNextIndex() {
	return size;
}

void* Archetype::getPointer(int component, int index) const {
	int idx = index % 64;
	void* componentArray = tables[index / 64]->buffer + offsetMap[component];
	return (char*)componentArray + (idx * ComponentSizes[component]);
}

void* Archetype::getComponentPointer(int itemIndex, int componentID) {
	assert(mask[componentID] && componentIndex[componentID] > -1);
	return tables[itemIndex / 64]->pointerGrid[componentIndex[componentID]][itemIndex % 64];
};

void* Archetype::getComponentParentPointer(int itemIndex, int componentID) const {
	assert(ComponentParentStatus[componentID]);
	return tables[itemIndex / 64]->GetComponentParentPointerArray(componentID)[itemIndex % 64];
}

EntityID Archetype::getEntityID(int index) {
	return *(EntityID*)getPointer(0, index);
}

void Archetype::setupOffsetMap() {
	cout << "Creating Offset Map for Level " << level << " Archetype: " << ComponentMaskAsString(mask) << endl;
	size_t current = 0;
	for (int i = 0; i < 512; i++) if (mask.test(i)) { offsetMap[i] = current; current = current + ComponentSizes[i] * 64; cout << "" << ComponentNames[i] << "\t" << offsetMap[i] << "\t" << ComponentSizes[i] << endl; }
	table_size = current;
}

Spot Archetype::switchArchetype(Spot source, Archetype* destinationArchetype) {
	Archetype* sourceArchetype = source.archetype;
	if (sourceArchetype == destinationArchetype) return source;
	int destinationIndex = destinationArchetype->allocateItem(sourceArchetype->getEntityID(source.index), false);
	for (int c = 0; c < 512; c++) {
		if (sourceArchetype->mask[c] && !destinationArchetype->mask[c])
			ComponentConstructors[c](destinationArchetype->getPointer(c, destinationIndex));
		if (sourceArchetype->mask[c] && destinationArchetype->mask[c])
			ComponentCopiers[c](sourceArchetype->getPointer(c, source.index), destinationArchetype->getPointer(c, destinationIndex));
	}
	Spot spot{ destinationArchetype, destinationIndex };
	destinationArchetype->updateEntityStorage(sourceArchetype->getEntityID(source.index), spot);
	sourceArchetype->deallocateItem(source.index, false);
	return spot;
}

void* Archetype::GetComponentArray(int tableIndex, int componentID) {
	return tables[tableIndex]->buffer + offsetMap[componentID];
}

void Archetype::deleteItem(Spot source) {
	source.archetype->deallocateItem(source.index, true);
	source.archetype->updateEntityStorage(source.archetype->getEntityID(source.index), { nullptr, -1 });
}

Spot Archetype::copyOrCreateItem(EntityID entityID, Spot source, Archetype* destinationArchetype) {
	Archetype* sourceArchetype = source.archetype;
	auto sourceMask = sourceArchetype ? sourceArchetype->mask : ComponentMask(0);
	if (sourceArchetype == destinationArchetype) return source;
	int destinationIndex = destinationArchetype->allocateItem(entityID, false);
	for (int c = 0; c < 512; c++) {
		if (sourceMask[c] && !destinationArchetype->mask[c])
			ComponentConstructors[c](destinationArchetype->getPointer(c, destinationIndex));
		if (sourceMask[c] && destinationArchetype->mask[c])
			ComponentCopiers[c](sourceArchetype->getPointer(c, source.index), destinationArchetype->getPointer(c, destinationIndex));
	}
	Spot spot{ destinationArchetype, destinationIndex };
	destinationArchetype->updateEntityStorage(entityID, spot);
	return spot;
}

ostream& operator <<(std::ostream& os, const Archetype& val);

ostream& operator <<(std::ostream& os, const Archetype& val) {
	os << "Count: " << val.size << ", Level: " << val.level << "\t" << ComponentMaskAsString(val.mask) << endl;
	vector<nlohmann::json> v;
	for (int i = 0; i < val.size; i++) {
		os << "EntityID: " << ((EntityID*)val.getPointer(0, i))->index << "|" << ((EntityID*)val.getPointer(0, i))->generation << "\t" << val.getPointer(0, i) << endl;
		for (int j = 0; j < val.components.size(); j++) {
			int c = val.components[j];
			nlohmann::json k;
			ToJSONArr[c](k, val.getPointer(c, i));
			os << c << "\t" << val.getPointer(c, i) << '\t' << k << endl;
		}
		if (!val.components.size()) os << "\tEMPTY" << endl;
		os << "Parent Pointer Resolve" << endl;
		for (auto c : ComponentParentList) {
			auto p = val.getComponentParentPointer(i, c);
			cout << p << endl;
			if (p) {
				nlohmann::json k;
				ToJSONArr[c](k, val.getComponentParentPointer(i, c));
				os << '\t' << p << "\t" << k << endl;
			}
		}
	}
	os << "-----------------------------" << endl;
	return os;
}