import { serve } from 'bun'
import index from './index.html'
import { resolve } from 'path'

const distPath = resolve(import.meta.dir, '../../../dist')
const webPath = resolve(import.meta.dir, '../../../web')

const server = serve({
  routes: {
    '/dist/*': async (req) => {
      const url = new URL(req.url)
      const filePath = resolve(distPath, url.pathname.replace('/dist/', ''))
      const file = Bun.file(filePath)
      if (await file.exists()) {
        return new Response(file, {
          headers: {
            'Content-Type': 'application/javascript',
            'Access-Control-Allow-Origin': '*',
            'Cross-Origin-Opener-Policy': 'same-origin',
            'Cross-Origin-Embedder-Policy': 'require-corp',
          },
        })
      }
      return new Response('Not found', { status: 404 })
    },
    '/web/*': async (req) => {
      const url = new URL(req.url)
      const filePath = resolve(webPath, url.pathname.replace('/web/', ''))
      const file = Bun.file(filePath)
      if (await file.exists()) {
        return new Response(file, {
          headers: {
            'Access-Control-Allow-Origin': '*',
            'Cross-Origin-Opener-Policy': 'same-origin',
            'Cross-Origin-Embedder-Policy': 'require-corp',
          },
        })
      }
      return new Response('Not found', { status: 404 })
    },
    '/*': index,
  },
  development: process.env.NODE_ENV !== 'production' && {
    hmr: true,
    console: true,
  },
})

console.log(`🚀 Server running at ${server.url}`)
