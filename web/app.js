let module;

let selected = -1;
let pendingPromotion = null;
let isWhitePlayer = true;

createModule().then(m => {
    module = m;
    module._initPosition();
    renderBoard();
});

const pieceImages = {
  P: "pieces/wp.png",
  N: "pieces/wn.png",
  B: "pieces/wb.png",
  R: "pieces/wr.png",
  Q: "pieces/wq.png",
  K: "pieces/wk.png",
  p: "pieces/bp.png",
  n: "pieces/bn.png",
  b: "pieces/bb.png",
  r: "pieces/br.png",
  q: "pieces/bq.png",
  k: "pieces/bk.png"
};

function pieceToLetter(piece) {
    switch (piece) {
        case 1: return "P";
        case 2: return "N";
        case 3: return "B";
        case 4: return "R";
        case 5: return "Q";
        case 6: return "K";

        case -1: return "p";
        case -2: return "n";
        case -3: return "b";
        case -4: return "r";
        case -5: return "q";
        case -6: return "k";

        default: return "";
    }
}

function renderBoard() {
    const boardDiv = document.getElementById("board");
    boardDiv.innerHTML = "";

    const flipped = isWhitePlayer;

    for (let i = 0; i < 64; i++) {

        const file = i % 8;
        const rank = Math.floor(i / 8);

        const mappedRank = flipped ? (7 - rank) : rank;
        const sqIndex = mappedRank * 8 + file;

        const sq = document.createElement("div");

        const drawRank = Math.floor(sqIndex / 8);
        const drawFile = sqIndex % 8;

        sq.className =
            "sq " + ((drawRank + drawFile) % 2 ? "dark" : "light");

        const piece = module._getPiece(sqIndex);
        const letter = pieceToLetter(piece);

        if (letter && pieceImages[letter]) {
            const img = document.createElement("img");
            img.src = pieceImages[letter];
            img.className = "piece";
            sq.appendChild(img);
        }

        // compare ENGINE square to ENGINE square
        if (sqIndex === selected) {
            sq.style.outline = "2px solid yellow";
        }

        sq.onclick = () => onSquare(sqIndex);

        boardDiv.appendChild(sq);
    }
}

function onSquare(i) {

    // block input during promotion
    if (pendingPromotion) return;

    // -------------------------
    // SELECT PIECE
    // -------------------------
    if (selected === -1) {
        const moveCount = module._selectSquare(i);
        console.log("legal moves:", moveCount);

        selected = i;
        renderBoard();
        return;
    }

    const from = selected;
    const to = i;

    // Use selected-move API consistently
    const moveCount = module._selectSquare(from);

    let promoMoves = [];

    // -------------------------
    // PROMOTION DETECTION
    // -------------------------
    for (let j = 0; j < moveCount; j++) {

        const mTo = module._getSelectedMoveTo(j);
        const flags = module._getSelectedMoveFlags(j);
        const promo = module._getSelectedMovePromo(j);

        if (mTo === to && (flags & 4)) {
            promoMoves.push({ from, to, promo });
        }
    }

    // debug
    for (let j = 0; j < moveCount; j++) {

        const mTo = module._getSelectedMoveTo(j);
        const flags = module._getSelectedMoveFlags(j);
        const promo = module._getSelectedMovePromo(j);

        console.log("RAW MOVE:", {
            to: mTo,
            flags,
            flagsType: typeof flags,
            flagsBin: (flags >>> 0).toString(2),
            promo
        });
    }

    console.log(module._evaluatePosition());

    // -------------------------
    // SHOW PROMOTION MENU
    // -------------------------
    if (promoMoves.length > 0) {
        pendingPromotion = promoMoves;
        document.getElementById("promotionMenu").style.display = "block";
        return;
    }

    // normal move
    const moveMade = module._makeMove(from, to);

    if (!moveMade) {
        selected = -1;
        renderBoard();
        return;
    }

    selected = -1;
    renderBoard();

    setTimeout(() => {
        module._makeAIMove();
        renderBoard();
    }, 250);
}

// -------------------------
// PROMOTION HANDLER
// -------------------------
window.choosePromotion = function(pieceType) {

    if (!pendingPromotion) return;

    const move = pendingPromotion.find(m => m.promo === pieceType);

    if (!move) return;

    module._makeMove(move.from, move.to, move.promo);

    pendingPromotion = null;
    document.getElementById("promotionMenu").style.display = "none";

    selected = -1;
    renderBoard();
};
