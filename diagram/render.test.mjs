import {test} from 'node:test';
import assert from 'node:assert/strict';
import {render,validate} from './render.ts';
test('rejects invalid boundary inputs',()=>{
  for(const v of [null,[],{}, {label:'x',notes:[]},{label:'x',notes:[59]}, {label:'x',notes:[72]}, {label:'x',notes:[60.5]}, {label:'x',notes:[60,60]}, {label:'x',notes:[60],extra:true}]) assert.throws(()=>validate(v));
});
test('escapes both accessible text and visible text',()=>{
  const svg=render({label:'<script>"&',notes:[60,64,67]});
  assert.ok(!svg.includes('<script>'));
  assert.ok(svg.includes('&lt;script&gt;&quot;&amp;'));
});
test('draws one octave and exactly selected notes',()=>{
  const svg=render({label:'C major',notes:[60,64,67]});
  assert.equal((svg.match(/<rect /g)||[]).length,12);
  assert.equal((svg.match(/#db5d42/g)||[]).length,3);
  assert.equal(svg,render({label:'C major',notes:[60,64,67]}));
});
test('validation copies the caller array',()=>{
  const input={label:'C',notes:[60]}; const result=validate(input);
  result.notes.push(64); assert.deepEqual(input.notes,[60]);
});
