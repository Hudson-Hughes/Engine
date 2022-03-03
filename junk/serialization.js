let fs = require('fs');
let path = require('path');
let schema = require(path.join(process.cwd(), "schema.json"));
let base = fs.readFileSync(path.join(process.cwd(), "SerializationBase.hpp")).toString();

// var filters = JSON.parse(fs.readFileSync(path.join(process.cwd(), "Filters.json")).toString()).filters;
// var parents = JSON.parse(fs.readFileSync(path.join(process.cwd(), "Filters.json")).toString()).parents;

for (var i = 0; i < schema.length; i++) schema[i]['index'] = i;

names = schema.map(x => x.name);
indexes = schema.map((x, i) => i);

type_names = schema.map(x => x.name);
modules = new Set(schema.map(x => x.module));

j = 0;
arrays = [
    `void (*ToJSONArr[${schema.length}])(nlohmann::json&, void*) = { ${indexes.map(x => `ToJSON<${x}>`).join(', ')} };`,
    `void (*FromJSONArr[${schema.length}])(nlohmann::json&, void*) = { ${indexes.map(x => `FromJSON<${x}>`).join(', ')} };`,
    `string ComponentNames[${schema.length}] = { ${names.map(x => `"${x}"`).join(', ')} };`,
    `void (*ComponentDestructors[${schema.length}])(void*) = { ${indexes.map(x => `Destroy<${x}>`).join(', ')} };`,
    `void* (*ComponentConstructors[${schema.length}])(void*) = { ${indexes.map(x => `Construct<${x}>`).join(', ')} };`,
    `void* (*ComponentCopiers[${schema.length}])(void*, void*) = { ${indexes.map(x => `Copy<${x}>`).join(', ')} };`,
    `void* (*ComponentCopyScorchers[${schema.length}])(void*, void*) = { ${indexes.map(x => `CopyScorch<${x}>`).join(', ')} };`,
    `size_t ComponentSizes[${schema.length}] = { ${names.map(x => `sizeof(${x})`).join(', ')} };`,
    // `bool ComponentParentStatus[${schema.length}] = { ${names.map(x => `${parents[x] ?? 0}`).join(', ')} };`,
    // `int ComponentParentIndex[${schema.length}] = { ${names.map((v, i) => {
    //     return parents[v] ? j++ : -1;
    // }).join(', ')} };`,
    // `int ComponentParentList[${Object.keys(parents).filter(x => parents[x]).length}] = { ${names.map((name, idx) => { return { n: name, i: idx } }).filter((item) => parents[item.n]).map(x => x.i).join(', ')} };`,
    // `int ParentComponentCount = ${Object.keys(parents).filter(x => parents[x]).length};`,
    'bool ComponentParentStatus[4] = { 0, 0, 0, 0 };',
    'int ComponentParentIndex[4] = { 0, 0, 0, 0 };',
    'int ComponentParentList[2] = { 0, 0 };',
    'int ParentComponentCount = 0;',
];

array_declarations = arrays
.map( x => 'extern ' + x.split(/\s+=\s+/g)[0] + ';');

array_declarations.push('typedef void* ParentPointerGrid[2][64];')

array_definitions = arrays;

// template_definitions = template_definitions.flatMap(y => schema.map(struct => y(struct)));
// template_declarations = template_definitions.map( x => x.split('\n')[0] + ';' );

fs.writeFileSync('../Engine/include/reflection.hpp', base + '\n');
fs.appendFileSync('../Engine/include/reflection.hpp', `#include "people.hpp"\n#include "entity.hpp"` + '\n');

// fs.appendFileSync('./serialization.hpp', modules.map(x => ('#include <' + x + '>')).join('\n') + '\n');
fs.appendFileSync('../Engine/include/reflection.hpp', schema.map( (struct, idx) => `REFLECTION_DECLARATION(${struct.name}, ${idx}, ${Object.keys(struct.fields).join(', ')})` ).join('\n') + '\n');
fs.appendFileSync('../Engine/include/reflection.hpp', array_declarations.join('\n') + '\n');

fs.writeFileSync('../Engine/src/reflection.cpp', '#include "reflection.hpp"' + '\n\n');
fs.appendFileSync('../Engine/src/reflection.cpp', schema.map( (struct, idx) => `REFLECTION_DEFINITION(${struct.name}, ${idx}, ${Object.keys(struct.fields).join(', ')})` ).join('\n') + '\n');
fs.appendFileSync('../Engine/src/reflection.cpp', array_definitions.join('\n') + '\n');
