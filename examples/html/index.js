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
const serverUrl = `http://localhost:${server.port}/`

const printHeader = (includeReadyTime = false) => {
  if (includeReadyTime) {
    console.log(
      `${prefix}${lightBlue}${bold}Bun v${Bun.version}${reset} ${dim}ready in${reset} ${bold}${readyTime}${reset} ms`,
    )
  } else {
    console.log(`${prefix}${lightBlue}${bold}Bun v${Bun.version}${reset}`)
  }
  console.log(`\n${lightBlue}➜${reset} ${cyan}${serverUrl}${reset}`)
}

const printStatus = (includeReadyTime = false) => {
  printHeader(includeReadyTime)
}

const printShortcuts = (includeReadyTime = false) => {
  printHeader(includeReadyTime)
  console.log(`\n  Shortcuts${dim}:${reset}`)
  console.log()
  console.log(`  ${dim}→${reset}   ${cyan}c + Enter${reset}   clear screen`)
  console.log(`  ${dim}→${reset}   ${cyan}o + Enter${reset}   open in browser`)
  console.log(`  ${dim}→${reset}   ${cyan}q + Enter${reset}   quit (or Ctrl+C)`)
  console.log()
}

printStatus(true)
console.log(
  `\n${dim}Press ${cyan}h + Enter${reset}${dim} to show shortcuts${reset}`,
)

process.stdin.resume()
process.stdin.setEncoding('utf8')

process.stdin.on('data', (key) => {
  if (key === '\u0003' || key === 'q\n' || key === 'q\r') {
    process.exit()
  }
  if (key === 'h\n' || key === 'h\r') {
    console.clear()
    printShortcuts()
  }
  if (key === 'c\n' || key === 'c\r') {
    console.clear()
    printStatus()
  }
  if (key === 'o\n' || key === 'o\r') {
    const { exec } = require('child_process')
    const command =
      process.platform === 'darwin'
        ? 'open'
        : process.platform === 'win32'
        ? 'start'
        : 'xdg-open'
    exec(`${command} ${serverUrl}`)
  }
})
