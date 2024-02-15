let previous = undefined;
let wasm = undefined;
let ctx = undefined;
let delta = undefined;

function make_environment(...envs) {
    return new Proxy(envs, {
        get(target, prop, receiver) {
            for (let env of envs) {
                if (env.hasOwnProperty(prop)) {
                    return env[prop];
                }
            }

            return (...args) => {console.error("Not Implemented: " + prop, args)};
        }
    });
}

function cstrlen(mem, ptr) {
    let len = 0;
    while (mem[ptr] != 0) {
        len++;
        ptr++;
    }
    return len;
}

function cstr_by_ptr(mem_buffer, ptr) {
    const mem = new Uint8Array(mem_buffer);
    const len = cstrlen(mem, ptr);
    return new TextDecoder().decode(new Uint8Array(mem_buffer, ptr, len));
}

function color_to_hex(r, g, b, a) {
    r = r.toString(16).padStart(2, '0');
    g = g.toString(16).padStart(2, '0');
    b = b.toString(16).padStart(2, '0');
    a = a.toString(16).padStart(2, '0');
    return `#${r}${g}${b}${a}`;
}

WebAssembly.instantiateStreaming(fetch('game.wasm'), {
    env: make_environment({
        InitWindow: (width, height, title_ptr) => {
            ctx.canvas.width = width;
            ctx.canvas.height = height;
            document.title = cstr_by_ptr(wasm.instance.exports.memory.buffer, title_ptr);
        },
        SetTargetFPS: (fps) => {
            console.log(`The game wants to run at ${fps} FPS, but we're ignoring it.`);
        },
        BeginDrawing: () => {
            ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);
        },
        EndDrawing: () => {
        },
        ClearBackground: (color_ptr) => {
            const buffer = wasm.instance.exports.memory.buffer;
            const [r, g, b, a] = new Uint8Array(buffer, color_ptr, 4);
            ctx.fillStyle = color_to_hex(r, g, b, a);
            ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
        },
        GetScreenWidth: () => ctx.canvas.width,
        GetScreenHeight: () => ctx.canvas.height,
        GetFrameTime: () => delta,
        DrawCircleV: (center_ptr, radius, color_ptr) => {
            const buffer = wasm.instance.exports.memory.buffer;
            const [x, y] = new Float32Array(buffer, center_ptr, 2);
            const [r, g, b, a] = new Uint8Array(buffer, color_ptr, 4);

            ctx.beginPath();
            ctx.arc(x, y, radius, 0, Math.PI * 2, false);
            ctx.fillStyle = color_to_hex(r, g, b, a);
            ctx.fill();
            ctx.drawImage()
        }
    })
}).then((w) => {
    wasm = w;
    const canvas = document.getElementById('game');
    ctx = canvas.getContext('2d');

    w.instance.exports.game_init();
    function first(timestamp) {
        previous = timestamp;
        window.requestAnimationFrame(next);
    }
    function next(timestamp) {
        delta = (timestamp - previous) / 1000.0;
        previous = timestamp;
        w.instance.exports.game_frame();
        window.requestAnimationFrame(next);
    }

    window.requestAnimationFrame(first);
}).catch((e) => console.error(e));