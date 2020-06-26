const NoteValues = {
  C: 0,
  D: 2,
  E: 4,
  F: 5,
  G: 7,
  A: 9,
  B: 11
};

function nameToValue(str) {
  const match = str.match(/^([A-G])([b#]*)(\d*)$/)

  if (!match) {
    throw `Invalid note name: ${str}`;
  }

  const noteValue = NoteValues[match[1]];
  let accidentals = 0;

  for (let accidental of match[2]) {
    switch (accidental) {
      case '#': accidentals++; break;
      case 'b': accidentals--; break;
    }
  }

  const octave = parseInt(match[3] || 4) + 1;

  return octave * 12 + noteValue + accidentals;
}

function notesToArray(str) {
  const values = [];

  for (let token of str.trim().split(/\s+/)) {
    switch (token) {
      case "OFF":
        values.push(-1);
        break;
      case "|":
        break;
      default:
        values.push(nameToValue(token));
    }
  }

  return values;
}

module.exports = { nameToValue, notesToArray };
