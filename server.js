// const express = require('express');
// const { spawn } = require('child_process');
// const path = require('path');
// const app = express();
// const port = 3000;

// app.use(express.static('public'));
// app.use(express.json());

// const cppDir = path.join(__dirname, 'cpp');
// const cppExecutable = path.join(cppDir, process.platform === 'win32' ? 'p1.exe' : 'p1');

// // Spawn the C++ process once at server start
// const cppProcess = spawn(cppExecutable, { cwd: cppDir });
// let outputBuffer = '';

// // Handle stdout
// cppProcess.stdout.on('data', (data) => {
//     outputBuffer += data.toString();
// });

// cppProcess.stderr.on('data', (data) => {
//     console.error(`C++ Error: ${data}`);
// });

// cppProcess.on('close', (code) => {
//     console.log(`C++ process exited with code ${code}`);
// });

// app.post('/execute', (req, res) => {
//     const { choice, input } = req.body;
//     outputBuffer = ''; // Reset buffer for new command

//     // Send command to C++ process
//     cppProcess.stdin.write(`${choice}\n`);
//     if (input) {
//         cppProcess.stdin.write(`${input}\n`);
//     }

//     // Wait briefly for output (adjust delay as needed)
//     setTimeout(() => {
//         if (outputBuffer) {
//             res.json({ output: outputBuffer });
//         } else {
//             res.status(500).json({ error: 'No output received' });
//         }
//     }, 100); // 100ms delay to allow C++ to process
// });

// app.get('/visualize', (req, res) => {
//     const dotFile = path.join(cppDir, 'parse_tree.dot');
//     const pngFile = path.join(cppDir, 'parse_tree.png');
//     const dotProcess = spawn('dot', ['-Tpng', dotFile, '-o', pngFile], { cwd: cppDir });

//     dotProcess.on('close', (code) => {
//         if (code === 0) {
//             res.sendFile(pngFile);
//         } else {
//             res.status(500).json({ error: 'Failed to generate parse tree image' });
//         }
//     });
// });

// app.listen(port, () => {
//     console.log(`Server running at http://localhost:${port}`);
// });

// // Graceful shutdown
// process.on('SIGINT', () => {
//     cppProcess.stdin.write('5\n'); // Send exit command (assuming 5 exits the loop)
//     cppProcess.on('close', () => process.exit());
// });



// server.js
import express from 'express';
import { spawn } from 'child_process';
import { join } from 'path';
import { fileURLToPath } from 'url';
import { dirname } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const app = express();
const port = 3000;

app.use(express.static('public'));
app.use(express.json());

const cppDir = join(__dirname, 'cpp');
const cppExecutable = join(cppDir, process.platform === 'win32' ? 'p1.exe' : 'p1');

// Spawn the C++ process once at server start
const cppProcess = spawn(cppExecutable, { cwd: cppDir });
let outputBuffer = '';

// Handle stdout
cppProcess.stdout.on('data', (data) => {
    outputBuffer += data.toString();
});

cppProcess.stderr.on('data', (data) => {
    console.error(`C++ Error: ${data}`);
});

cppProcess.on('close', (code) => {
    console.log(`C++ process exited with code ${code}`);
});

// app.post('/execute', async (req, res) => {
//     const { choice, input } = req.body;
//     outputBuffer = ''; // Reset buffer for new command

//     // Send command to C++ process
//     cppProcess.stdin.write(`${choice}\n`);
//     if (input) {
//         cppProcess.stdin.write(`${input}\n`);
//     }

//     // Wait briefly for output (adjust delay as needed)
//     await new Promise(resolve => setTimeout(resolve, 100)); // 100ms delay to allow C++ to process
//     if (outputBuffer) {
//         res.json({ output: outputBuffer });
//     } else {
//         res.status(500).json({ error: 'No output received' });
//     }
// });

app.post('/execute', async (req, res) => {
    const { choice, input } = req.body;
    outputBuffer = ''; // Reset buffer for new command

    // Send command to C++ process
    cppProcess.stdin.write(`${choice}\n`);
    if (input) {
        cppProcess.stdin.write(`${input}\n`);
    }

    // Wait briefly for output (adjust delay as needed)
    await new Promise(resolve => setTimeout(resolve, 500)); // 100ms delay to allow C++ to process
    if (outputBuffer) {
        res.json({ output: outputBuffer });
    } else {
        res.status(500).json({ error: 'No output received' });
    }
});

app.get('/visualize', async (req, res) => {
    const dotFile = join(cppDir, 'parse_tree.dot');
    const pngFile = join(cppDir, 'parse_tree.png');
    const dotProcess = spawn('dot', ['-Tpng', dotFile, '-o', pngFile], { cwd: cppDir });

    dotProcess.on('close', (code) => {
        if (code === 0) {
            res.sendFile(pngFile);
        } else {
            res.status(500).json({ error: 'Failed to generate parse tree image' });
        }
    });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});

// Graceful shutdown
process.on('SIGINT', () => {
    cppProcess.stdin.write('7\n'); // Updated to choice 7 (exit) based on new menu
    cppProcess.on('close', () => process.exit());
});