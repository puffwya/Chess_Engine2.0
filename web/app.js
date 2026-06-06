const ModulePromise = createModule();

ModulePromise.then((Module) => {

    Module._initPosition();

    const boardDiv = document.getElementById("board");

    boardDiv.style.display = "grid";
    boardDiv.style.gridTemplateColumns = "repeat(8, 60px)";
    boardDiv.style.width = "480px";

    let selectedSquare = -1;
    let moveCount = 0;

    const pieceMap = {
        1: "P",
        2: "N",
        3: "B",
        4: "R",
        5: "Q",
        6: "K",
        "-1": "p",
        "-2": "n",
        "-3": "b",
        "-4": "r",
        "-5": "q",
        "-6": "k"
    };

    function draw() {
        boardDiv.innerHTML = "";

        for (let i = 0; i < 64; i++) {
            const sq = document.createElement("div");

            const row = Math.floor(i / 8);
            const col = i % 8;

            sq.style.width = "60px";
            sq.style.height = "60px";
            sq.style.display = "flex";
            sq.style.alignItems = "center";
            sq.style.justifyContent = "center";
            sq.style.fontSize = "32px";
            sq.style.cursor = "pointer";

            sq.style.background =
                (row + col) % 2 === 0 ? "#eeeed2" : "#769656";

            // highlight selected square
            if (i === selectedSquare) {
                sq.style.outline = "3px solid yellow";
            }

            const piece = Module._getPiece(i);
            sq.textContent = pieceMap[piece] || "";

            sq.onclick = () => onSquareClick(i);

            boardDiv.appendChild(sq);
        }
    }

    function onSquareClick(i) {

        // -------------------------
        // STEP 1: select square
        // -------------------------
        const count = Module._selectSquare(i);

        // if clicking same square → deselect
        if (selectedSquare === i) {
            selectedSquare = -1;
            draw();
            return;
        }

        // if we already selected a piece → attempt move
        if (selectedSquare !== -1) {

            const from = selectedSquare;
            const to = i;

            Module._makeMove(from, to);

            selectedSquare = -1;
            draw();
            return;
        }

        // otherwise select new square
        selectedSquare = i;
        moveCount = count;

        console.log("moves for square:", count);

        draw();
    }

    draw();
});
