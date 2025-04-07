function showSection(sectionId) {
    document.querySelectorAll('.section').forEach(section => {
        section.style.display = 'none';
    });
    document.getElementById(sectionId).style.display = 'block';
    if (sectionId === 'symbol-table') refreshSymbolTable();
}

async function declareVariables() {
    const declaration = document.getElementById('declInput').value;
    const response = await fetch('/execute', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ choice: 2, input: declaration })
    });
    const data = await response.json();
    document.getElementById('declResult').textContent = data.output || data.error;
}

async function evaluateExpression() {
    const expression = document.getElementById('exprInput').value;
    const response = await fetch('/execute', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ choice: 1, input: `${expression};` })
    });
    const data = await response.json();
    document.getElementById('exprResult').textContent = data.output || data.error;
}

async function refreshSymbolTable() {
    const response = await fetch('/execute', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ choice: 3 })
    });
    const data = await response.json();
    const output = data.output || data.error;
    // Parse the symbol table output (this is a simple example; adjust based on actual output)
    const tbody = document.getElementById('symbolTableBody');
    tbody.innerHTML = '';
    const lines = output.split('\n').filter(line => line.includes('\t'));
    lines.forEach(line => {
        const [name, type, value, scope] = line.split('\t').map(s => s.trim());
        if (name && type && value && scope) {
            const row = document.createElement('tr');
            row.innerHTML = `
                <td>${name}</td>
                <td>${type}</td>
                <td>${value}</td>
                <td>${scope}</td>
            `;
            tbody.appendChild(row);
        }
    });
}
// ... (previous functions remain the same)

async function visualizeParseTree() {
    const response = await fetch('/visualize');
    if (response.ok) {
        const blob = await response.blob();
        const url = URL.createObjectURL(blob);
        document.getElementById('parseTreeImg').src = url;
        showSection('visualize');
    } else {
        alert('Error generating parse tree image');
    }
}