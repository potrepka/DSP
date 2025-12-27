const startTime = performance.now()
const isDev = process.env.NODE_ENV === 'development'

const server = Bun.serve({
  port: 3000,
  async fetch(req) {
    const url = new URL(req.url)
    let path = url.pathname
    if (path === '/') {
      path = '/index.html'
    }
    const file = Bun.file(import.meta.dir + path)
    if (await file.exists()) {
      return new Response(file)
    }
    return new Response('Not Found', { status: 404 })
  },
})

const readyTime = (performance.now() - startTime).toFixed(2)
const reset = '\x1b[0m'
const dim = '\x1b[2m'
const cyan = '\x1b[36m'
const lightBlue = '\x1b[94m'
const bold = '\x1b[1m'
const bgBlue = '\x1b[44m'
const black = '\x1b[30m'

const prefix = isDev ? `${bgBlue}${black} DEV ${reset} ` : ''
console.log(
  `${prefix}${lightBlue}${bold}Bun v${Bun.version}${reset} ${dim}ready in${reset} ${bold}${readyTime}${reset} ms\n`,
)
console.log(
  `${lightBlue}➜${reset} ${cyan}http://localhost:${server.port}/${reset}\n`,
)
console.log(
  `${dim}Press ${cyan}h + Enter${reset}${dim} to show shortcuts${reset}`,
)
