// Render explicit MIDI pitches. Chord naming and voicing selection belong
// in a separate theory layer. Validate before generating any SVG.
export function validate(input: unknown): {label: string; notes: number[]} {
  if (!input || typeof input !== 'object' || Array.isArray(input)) throw new TypeError('Expected object');
  if (Object.keys(input).some(k => !['label', 'notes'].includes(k))) throw new TypeError('Unknown field');
  const {label, notes} = input as Record<string, unknown>;
  if (typeof label !== 'string' || label.length > 80) throw new TypeError('Label must be at most 80 characters');
  if (!Array.isArray(notes) || notes.length < 1 || notes.length > 12 ||
      notes.some(n => !Number.isInteger(n) || n < 60 || n > 71) || new Set(notes).size !== notes.length)
    throw new TypeError('Supply 1–12 distinct MIDI notes from 60 through 71');
  return {label, notes: [...notes]};
}
const entities: Record<string, string> = {'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&apos;'};
const escapeXml = (s: string): string => s.replace(/[&<>"']/g, c => entities[c]!);
export function render(input: unknown): string {
  const {label, notes} = validate(input);
  const whites = [60,62,64,65,67,69,71], blacks = [[61,25],[63,65],[66,145],[68,185],[70,225]];
  const key = (n: number,x: number,w: number,h: number) => `<rect x="${x}" y="45" width="${w}" height="${h}" fill="${notes.includes(n)?'#db5d42':h===100?'#202623':'#fff'}" stroke="#202623"/>`;
  return `<svg xmlns="http://www.w3.org/2000/svg" role="img" aria-label="${escapeXml(label)}" viewBox="0 0 280 220"><title>${escapeXml(label)}</title><text x="8" y="28" font-family="sans-serif">${escapeXml(label)}</text>${whites.map((n,i)=>key(n,i*40,40,160)).join('')}${blacks.map(([n,x])=>key(n,x,30,100)).join('')}</svg>`;
}
