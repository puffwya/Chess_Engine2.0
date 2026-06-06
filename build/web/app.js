let module;

let selected = -1;
let pendingPromotion = null;

createModule().then(m => {
    module = m;

    module._initPosition();

    renderBoard();
});

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

    for (let i = 0; i < 64; i++) {
        const sq = document.createElement("div");

        sq.className =
            "sq " + ((Math.floor(i / 8) + i) % 2 ? "dark" : "light");

        const piece = module._getPiece(i);

        sq.innerText = pieceToLetter(piece);

        if (i === selected) {
            sq.style.outline = "2px solid yellow";
        }

        sq.onclick = () => onSquare(i);

        boardDiv.appendChild(sq);
    }
}

function onSquare(i) {

    // block moves while promotion is pending
    if (pendingPromotion) return;

    if (selected === -1) {
        const moveCount = module._selectSquare(i);
        console.log("legal moves:", moveCount);

        selected = i;
        renderBoard();
        return;
    }

    const from = selected;
    const to = i;

    // reselect moves from engine
    const moveCount = module._selectSquare(from);

    let promoMoves = [];

    for (let j = 0; j < moveCount; j++) {

        const mTo = module._getMoveTo(j);
        const flags = module._getMoveFlags(j);
        const promo = module._getMovePromo(j);

        if (mTo === to && (flags & 4)) {
            promoMoves.push({ from, to, promo });
        }
    }

    // -------------------------
    // PROMOTION ONLY ADDITION
    // -------------------------

    for (let j = 0; j < moveCount; j++) {
        const to = module._getMoveTo(j);
        const flags = module._getMoveFlags(j);
        const promo = module._getMovePromo(j);

        console.log("RAW MOVE:", {
            to,
            flags,
            flagsType: typeof flags,
            flagsBin: (flags >>> 0).toString(2),
            promo
        });
    }

    if (promoMoves.length > 0) {
        pendingPromotion = promoMoves;
        document.getElementById("promotionMenu").style.display = "block";
        return;
    }

    module._makeMove(from, to);

    selected = -1;
    renderBoard();
}

// -------------------------
// PROMOTION BUTTON HOOK
// -------------------------
window.choosePromotion = function(pieceType) {

    if (!pendingPromotion) return;

    const move = pendingPromotion.find(m => m.promo === pieceType);

    if (!move) return;

    module._makeMove(move.from, move.to);

    pendingPromotion = null;
    document.getElementById("promotionMenu").style.display = "none";

    selected = -1;
    renderBoard();
};
