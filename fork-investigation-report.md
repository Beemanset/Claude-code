# Fork investigation report: UK spot, long-only, no-leverage crypto bot

_Research date: 1 Oct 2026. Read-only: nothing was forked, starred, run, or contacted, and no API keys were used._

## How this was done, and its limits

- **Fork enumeration.** The GitHub REST forks endpoint is still blocked from this session: `gh` has no valid token, and the proxy only allows repository-scoped API calls for attached repos. I read the public forks page (`github.com/imikerussell/beebots/forks`, 4 pages) instead. It lists **94 forks**. Upstream reports **95**; the missing one is most likely `danielcfho/beebots`, which `git ls-remote` can't read (it asks for credentials, so it's private or deleted).
- **Divergence test.** For every fork I ran anonymous `git ls-remote` and compared each branch head with the upstream commit list. **86 forks** have every branch pointing at an upstream commit, so they're identical to upstream or simply behind it. **8 forks have diverged.** I fetched those and diffed them against `imikerussell:main` locally. This is equivalent to the `compare` API the brief asks for.
- **Forks of forks.** I checked the `/forks` page of all 8 diverged forks. None has been forked.
- **PRs.** Upstream has 0 PRs, open or closed. `WGDCOSTA` has 10 internal PRs (9 merged, #10 open). `Namanyahillary` has 0. `MatthewMastando` has one internal merge (PR #1).
- **Blocked sites.** These were blocked by the egress proxy, so facts about them come from web-search excerpts rather than first-hand reads: `freqtrade.io` (I read the docs from the git repo instead), `jesse.trade`, `kraken.com`, `docs.kraken.com`, `help.coinbase.com`, `docs.cdp.coinbase.com`. Each such fact is marked "(search)" below.
- **Missing brief.** `uk-spot-bees-brief.md` isn't in this environment. I judged effort against the requirements stated in this task: spot, long only, no leverage, UK retail legal, self-hosted, slow strategy.

---

## 1. Summary table

### 1a. beebots: upstream plus the 8 diverged forks

| Repo / branch | Live venue (where real orders go) | Product | Strategy | Decision model | Maturity (commits ahead · test files · last commit) | UK/FCA mention | UK-usable |
|---|---|---|---|---|---|---|---|
| [imikerussell/beebots](https://github.com/imikerussell/beebots) `main` (upstream) | OKX EEA | Perps (X-Perps), up to 2x, long and short | Breakout, trend (Donchian ensemble), momentum | Jev + rule risk layer | 9 commits total · 17 · 27 Sep. Paper by default. | None ("check that OKX's derivatives are available where you live") | **No.** Needs a new venue and spot rewrite. |
| [Namanyahillary/beebots](https://github.com/Namanyahillary/beebots) `future/profit-mode` | OKX EEA perps. **Alpaca spot is paper only.** | Perps live; spot long-only 1x on Alpaca paper | 7 bees: breakouts, Bollinger-RSI mean reversion, crowded-funding fade, scalper retired with a postmortem | Jev + rules, a no-Jev shadow arm, Jev answer-reuse gate | 75 ahead · 30 · 29 Sep 22:41 UTC | None | **With changes.** Its `src/exec/alpaca.ts` (138 lines) shows how a spot executor plugs in. Whether Alpaca crypto is open to UK retail is unverified. |
| [WGDCOSTA/beebots](https://github.com/WGDCOSTA/beebots) `main`, `claude/beebots-intelligent-system-chrnrx`, `codex/jev-evidence-ledger-j2` | OKX (swap, isolated margin). Alpaca, Binance, Kraken and Coinbase are **data only**. | Perps | Many "skills", a strategy lab, macro squad, scalper, gold breakout research | Jev plus several LLM "brains" (Claude, Z.ai GLM, custom) | 32–42 ahead · 60 · **30 Sep 22:58 UTC** | None. Its legal outline is for an **Irish** operator (EU GDPR). | **No.** A multi-user platform, far beyond the scope. |
| [pjbish/beebots](https://github.com/pjbish/beebots) `bondi-bees` | OKX EEA perps | Perps | Upstream styles + `CALM_MODE` (no forced entries) + **21-week EMA regime filter** + profit lock from 1R | Jev, routed through Vercel AI Gateway | 5 ahead · 17 (+`regime.test.ts`) · 28 Sep | None | **With changes.** `src/bees/regime.ts` (72 lines) is worth reusing. |
| [psvanzyl/beebots](https://github.com/psvanzyl/beebots) `main` | OKX, **live real money** (about $37 per bee) | Perps | Upstream, then reverted to upstream risk defaults | Local Laya model on a home GPU, through `jev-shim` | 14 ahead · 16 · 29 Sep | None | **No.** Useful only as a pattern for a self-hosted Jev-compatible endpoint. |
| [Sellwy/beebots](https://github.com/Sellwy/beebots) `main` | OKX EEA perps | Perps | Upstream | Jev through OpenRouter (`typesafe/jev-router`); OpenAI optional | 2 ahead · 17 · 27 Sep | None | **No** (setup changes only) |
| [MatthewMastando/profitbots](https://github.com/MatthewMastando/profitbots) `main` + 3 `devin/*` branches | **Coinbase Derivatives** (US, CFTC-regulated FCM) | Futures, "up to 10x intraday" | One agent; the `all-assets` branch replaces the styles with ICT and volume-profile lenses | Jev | 7–8 ahead · 13 · 27 Sep | Says it's for US residents and that offshore venues aren't open to US residents | **No** |
| [Irwan6/TuyulBot](https://github.com/Irwan6/TuyulBot) `main` **(new)** | OKX EEA perps **or Hyperliquid perps** (EIP-712 agent-wallet signing) | Perps | Upstream styles | Jev | 6 ahead · 17 (+2 Hyperliquid tests) · 27 Sep | None. It exists because some Indonesian ISPs block OKX. | **No.** Hyperliquid perps are still derivatives. |
| [TulioAgents/tulio-beebots](https://github.com/TulioAgents/tulio-beebots) `main` **(new)** | OKX EEA perps | Perps | Upstream | Jev | 2 ahead (Makefile, `.serena` config) · 17 · 29 Sep | None | **No** (tooling only) |
| `joshcohen-ai/beebots` | — | — | — | — | Identical to upstream `5ddd6d1` | — | — |
| `henri-edh/beebots` | — | — | — | — | Shows as "updated 29 Sep" but is identical to upstream `5ddd6d1` | — | — |
| `danielcfho/beebots` | ? | ? | ? | ? | Not readable (private or deleted) | ? | Unknown |

**No beebots fork, and not upstream either, mentions the UK or the FCA, and none places real spot orders.** Every live order path is a derivative: OKX perps, Hyperliquid perps or Coinbase futures.

**Changes since 30 Sep 2026:**
- `WGDCOSTA` is the only fork with commits on or after 30 Sep:
  - `main` gained a Jev experiment ledger and evidence gate, a decision-stream redesign, and an "Arena plan" document.
  - `claude/beebots-intelligent-system-chrnrx` (PR #10, open) gained 10 more commits building Arena phase 1: magic-link accounts, one SQLite file per user, members' bots running on paper, a season leaderboard, consent records and draft legal pages for an Irish operator.
  - The venue hasn't changed.
- `Namanyahillary`'s last commit is 30 Sep 01:41 +0300 (29 Sep 22:41 UTC). Nothing is new since.
- Two forks the previous session didn't check have diverged: `Irwan6/TuyulBot` and `TulioAgents/tulio-beebots`. Neither helps.

The other 84 forks are identical to upstream or behind it. The full list is in the appendix.

### 1b. Established alternatives

| Project | Licence | Activity (last commit · latest release) | UK spot venue | Docker | Backtest / paper | Slow BTC/ETH daily trend fit | External decision model (LLM / Jev) | UK-usable |
|---|---|---|---|---|---|---|---|---|
| [Freqtrade](https://github.com/freqtrade/freqtrade) | GPL-3.0 | 29 Sep 2026 · `2026.9` (monthly releases) · about 33k commits, 55k stars | **Kraken spot officially supported.** Coinbase isn't on the supported list (only "potentially many others" via ccxt, with no guarantee). | Yes (`docs/docker_quickstart.md`) | Yes and yes (`dry_run` defaults to `true`) | Excellent. The `IStrategy` timeframe can be `1d`, and `trading_mode` defaults to spot. Official strategy repo has nothing exactly matching (`TrendRiderStrategy` is 1h with 1d/4h informative; `MultiMa` is 4h). A Donchian or EMA daily long-only strategy is about 50–100 lines. | Not built in. A strategy is Python, so it can call an HTTP endpoint, but such calls can't be backtested faithfully. FreqAI covers ML models. | **Yes, with changes** (config + strategy) |
| [Jesse](https://github.com/jesse-ai/jesse) | MIT | 27 Sep 2026 · v3.2.3 | Exchange enums include **"Kraken Pro Spot"** and **"Coinbase Spot"** | Yes (`salehmir/jesse`) | Yes and yes | Very good (clean strategy API, no look-ahead bias) | Jesse MCP server for AI *assistants*, not for runtime decisions | **Uncertain.** Live trading needs the separate `jesse_live` plugin and a `LICENSE_API_TOKEN` (in `jesse/services/auth.py`, `installer.py`). Pricing couldn't be checked because `jesse.trade` is blocked; historically it was a paid subscription. |
| [Hummingbot](https://github.com/hummingbot/hummingbot) | Apache-2.0 | 22 Sep 2026 · v2.17.0 | `kraken` and `coinbase_advanced_trade` spot connectors in the tree | Yes (Compose) | Paper connector yes; backtesting limited (mainly through the Dashboard and V2 controllers) | Weak. It's built around market making and HFT; directional and DCA executors exist but daily trend isn't its focus. | "Condor" AI harness for agentic strategies (README) | Yes, but a poor fit for the style |
| [OctoBot](https://github.com/Drakkar-Software/OctoBot) | GPL-3.0 | 21 Sep 2026 · 3.0.0-beta2 / 2.1.1 | Coinbase spot listed in the README; Kraken referenced throughout the code | Yes | Yes and yes | OK. DCA, grid, TradingView signals and indicator evaluators; a daily trend is assembled from evaluators. | **Best of the four.** A GPT/LangChain service with `LLM_CUSTOM_BASE_URL` (OpenAI-compatible, Ollama documented), so it could point at a Jev or Laya shim if that shim speaks OpenAI format. | Yes, with changes. The 3.0 line is beta. |
| [NautilusTrader](https://github.com/nautechsystems/nautilus_trader) (comparable) | LGPL-3.0 | 1 Oct 2026 · v2.0.0rc5 | `kraken` and `coinbase` adapters present | Yes | Yes (event-driven, high fidelity), sandbox adapter | Fine, but it's a professional framework: more code and a steeper learning curve | Not built in | Yes, but heavy for this purpose |

---

## 2. Exchanges (Task 3)

| | **Kraken (Payward Ltd, UK)** | **Coinbase (CB Payments Ltd, UK)** |
|---|---|---|
| FCA cryptoasset register (MLR) | Registered **22 Nov 2021** as a cryptoasset business. Payward Services Ltd separately holds an e-money authorisation, FRN 1010381. (search: [Kraken GB disclosures](https://www.kraken.com/gb/legal/disclosures)) | Registered **Jan/Feb 2025**, FRN **900635**. (search: [CoinDesk](https://dev.coindesk.com/policy/2025/02/03/coinbase-secures-spot-on-uk-crypto-register), [Cointelegraph](https://cointelegraph.com/news/coinbase-fca-uk-crypto-registration)) |
| Spot via API for UK retail | Kraken Pro REST/WS spot API; UK retail accounts are served by Payward Ltd. **Not verified first-hand** (kraken.com is blocked). | Advanced Trade API. **Not verified first-hand** (Coinbase docs are blocked). |
| Maker / taker (lowest tier) | **Conflicting.** Sources say the July 2026 schedule starts at **0.40% / 0.80%**, older sources say **0.25% / 0.40%**, and one search summary said 0.09% / 0.16%. (search: [datawallet](https://www.datawallet.com/crypto/kraken-fees-explained), [dollarscout](https://www.dollarscout.net/crypto/reviews/kraken/)) Check the live fee page before relying on any figure. | **0.40% / 0.60%** below $10k a month; 0% maker on stablecoin pairs. (search: [BrokerChooser](https://brokerchooser.com/broker-reviews/coinbase-review/coinbase-fees)) |
| Sandbox | **No public spot sandbox.** Spot UAT is "available on request" through an account manager; only Futures has a self-service demo (`demo-futures.kraken.com`). (search: [Kraken support](https://support.kraken.com/hc/en-us/articles/360024809011-API-Testing-Environment)) | **A static, mocked sandbox** (`api-sandbox.coinbase.com`): accounts and orders endpoints only, with canned responses. Useful for plumbing tests, not for paper trading. (search: [CDP docs](https://docs.cdp.coinbase.com/coinbase-app/advanced-trade-apis/sandbox)) |
| Trade + read only, no withdraw | Yes. Permissions are chosen per key, and "Withdraw Funds" is a separate permission you can leave off. (search: [Kraken API keys](https://docs.kraken.com/exchange/guides/rest/api-keys)) | Yes. CDP keys have separate **View / Trade / Transfer** scopes, so leave Transfer off. `/api/v3/brokerage/key_permissions` reports the scopes. (search: [CDP permissions](https://docs.cloud.coinbase.com/advanced-trade/docs/rest-api-scopes)) |
| IP allowlist | Yes, per key. (search, as above) | Yes, per key, up to 30 IPs. (search, as above) |

**What this means in practice:** paper trading on either exchange has to be **bot-side simulation against live prices** (Freqtrade `dry_run`, Jesse or OctoBot paper, or a beebots-style paper ledger). Neither exchange offers a spot paper venue like Alpaca's.

---

## 3. Shortlist: top 3 starting points

### 1. Freqtrade + Kraken spot + a custom daily trend strategy (recommended)
**Why it fits:**
- It's the most mature option by a wide margin: monthly releases, about 33k commits, active today.
- Spot is the default `trading_mode`, and leverage and shorting are off unless you configure them.
- Kraken spot is officially supported.
- Docker, dry-run, backtesting, hyperopt, look-ahead and recursive-bias checks, a web UI and Telegram are all built in.
- The upstream beebots trend bee is already based on Zarattini, Pagani & Barbon (2025). That paper's published rules are a **daily, long-only Donchian ensemble with vol-targeting** (see `strategies/BREEZY_BEE.md`). Beebots added the 4h bars and the short side. The paper's original version maps straight onto a Freqtrade `1d` strategy.

**Work needed (roughly 1–3 days, then weeks of dry-run):**
- Write one strategy file. Either the Donchian ensemble, or EMA-crossover + 21-week EMA regime (the idea in pjbish's `regime.ts`). About 50–150 lines of Python.
- Config: `exchange: kraken`, `trading_mode: spot`, BTC/GBP or BTC/USD and ETH pairs, `stake_amount`, `max_open_trades: 2`, `stoploss_on_exchange` (Kraken supports it).
- Backtest data: Kraken's API returns only 720 candles, so you need `--dl-trades` (slow and RAM-heavy) or Kraken's quarterly trade CSVs (`docs/exchanges.md`). Daily bars from another source would work for research only.
- Kraken has no spot sandbox, so run dry-run for several weeks, then go live with a small stake.
- Requirement gaps: no Jev or LLM out of the box. GPL-3.0 only matters if you distribute it.

### 2. Upstream beebots, rebuilt for spot, borrowing from forks (the original brief's path)
**Why it fits:** keeps Jev, the bee concept, the dashboard and the risk layer, and you already know the codebase. Useful pieces already exist:
- **`Namanyahillary` `src/exec/alpaca.ts` + `docs/ALPACA_PAPER.md`:** a working spot, long-only, 1x executor behind the existing `executor.ts` switch. It's the template for a `kraken-spot` executor.
- **`pjbish` `src/bees/regime.ts` + `test/regime.test.ts`:** the 21-week EMA regime filter, which fails closed. Also `CALM_MODE` (no forced entries).
- **`Irwan6` `src/market/venue.ts`:** a clean `VENUE` switch for market data across exchanges.
- **`psvanzyl` `jev-shim` / `Sellwy` OpenRouter:** cheaper or self-hosted ways to run Jev.

**Work needed (roughly 1–3 weeks):**
- A new Kraken (or Coinbase) spot executor and public-data client, with tests.
- Remove short sides, funding, margin and leverage from the bees, risk layer and ledger (the ledger is built around perps P&L and funding bills).
- Restrict Jev's menus to long and flat.
- Switch the trend bee back to daily bars and a long-only rule.
- Rework Setup, which currently checks against the OKX EEA perps list.
- A spot paper ledger, because Kraken has no sandbox.
- Ongoing cost and dependency: Jev is a proprietary TypeSafe AI model.

None of these forks has been merged upstream, so expect to hand-port the code rather than merge it.

### 3. OctoBot (only if an LLM in the loop matters)
**Why it fits:**
- GPL-3.0, Docker, paper trading and backtesting, Coinbase and Kraken spot.
- The only mainstream bot with a configurable OpenAI-compatible LLM endpoint (`LLM_CUSTOM_BASE_URL`, Ollama documented). Jev or Laya behind an OpenAI-format shim could act as an "evaluator".

**Work needed (roughly 3–7 days):**
- Compose a daily trend from TA evaluators, or use DCA plus a regime gate.
- Check how the GPT evaluator behaves in backtests.
- The 3.0 line is beta, so pin 2.1.x or test 3.0 carefully.
- Less transparent than Freqtrade for auditing exactly why a trade happened.

(Jesse would rank alongside OctoBot on design quality, but live trading depends on a licensed plugin I couldn't price. Hummingbot is a poor fit for slow trend trading.)

---

## 4. Recommendation

**Adapt Freqtrade. Don't adapt a beebots fork.**

- **None of the 95 forks gets you to UK-legal spot live trading.**
  - The closest is `Namanyahillary`, but its spot path is paper-only on Alpaca, and its live path and most of its 75 commits are about perps scalping and breakouts. That's the opposite of a slow, conservative strategy.
  - Every other diverged fork is still perps-only (OKX, Hyperliquid or Coinbase Derivatives) or only changes tooling.
- **Freqtrade already does the parts that matter:**
  - spot, Kraken, Docker
  - dry-run and backtesting
  - exchange-side stop-losses
  - years of edge-case fixes
- **Suggested route:**
  1. Encode the daily long-only rule from the paper beebots' trend bee cites, with a 21-week EMA regime gate.
  2. Backtest it on BTC and ETH.
  3. Dry-run it for 4–8 weeks.
  4. Go live with a small GBP stake on a trade-only, IP-allowlisted Kraken key.
- **If you want Jev's judgement later,** add it as a *veto or confirm* step on top of the rule-based signal. Either use a Freqtrade `confirm_trade_entry` callback that calls Jev, or run the beebots-spot rebuild (option 2) side by side on paper, the way Namanyahillary's shadow arm compares Jev with fixed rules.
- **Build from upstream beebots (option 2) only if Jev making the decisions is the point of the project,** not just a nice-to-have.

---

## 5. Unknowns and blockers

**Legal and regulatory**
- **Spot only.** The FCA's ban (PS20/10, in force 6 Jan 2021) covers *firms* selling, marketing or distributing crypto derivatives and ETNs to UK retail customers. ([FCA press release](https://fca.org.uk/news/press-releases/fca-bans-sale-crypto-derivatives-retail-consumers), [PS20/10](https://www.fca.org.uk/publication/policy/ps20-10.pdf)) Every beebots fork's live path depends on derivatives sold to retail. Hyperliquid (Irwan6) is a non-UK DEX outside FCA supervision; using it from the UK is a grey area I haven't researched, and it's derivatives anyway, so it's excluded.
- **The regime is changing.** The FCA's new FSMA cryptoasset authorisation regime opened its application window on **30 Sep 2026** (open until 28 Feb 2027) and takes effect on **25 Oct 2027**. Current MLR registrations don't carry over automatically. ([Burges Salmon](https://www.burges-salmon.com/our-thinking/the-uk-cryptoasset-regulatory-regime-where-we-are-now/), [Baker McKenzie](https://www.bakermckenzie.com/en/insight/publications/2026/02/united-kingdom-new-cryptoassets-regime-published), [Gherson](https://www.gherson.com/blog/uk-cryptoasset-regulation-2026-legal-update/)) Before Oct 2027, check that your chosen exchange has applied, or will be authorised, so your account isn't affected.
- **Running your own bot** on your own account isn't a regulated activity in itself. That's my understanding, not verified legal advice. It would change if you ran the bot for other people (WGDCOSTA's "Arena" model would).
- **UK onboarding rules** (financial promotions regime: appropriateness test, 24-hour cooling-off for first-time investors) apply when you open the exchange account, not to the bot.
- **Tax:** every bot sell is a CGT disposal subject to HMRC share-pooling rules. Keep the bot's trade ledger and export it.

**Exchange facts not verified first-hand (sites blocked here)**
- Kraken's current spot fee tier: sources conflict (0.40/0.80 versus 0.25/0.40). This affects whether short holding periods are viable. A daily strategy with a few trades a month can absorb either.
- Kraken's FRN for its cryptoasset registration, and confirmation that UK retail accounts get Kraken Pro API spot access with no restrictions. Check the [FCA register](https://register.fca.org.uk/) directly.
- Coinbase Advanced Trade API availability for UK retail accounts (expected, but not read first-hand).
- Whether Alpaca crypto is available to UK residents. Search was inconclusive: Alpaca entered the UK through WealthKernel in 2025, but its crypto offering looked US-focused. This only matters if you wanted Namanyahillary's Alpaca path to go live.

**Software**
- Jesse live-trading licence cost (`jesse.trade` is blocked).
- Freqtrade's maintenance status and features were read from its repo at commit date 29 Sep 2026 (release `2026.9`), not from the docs site.
- `danielcfho/beebots` couldn't be read.
- The 95th fork wasn't on the forks page (probably `danielcfho`).
- `uk-spot-bees-brief.md` wasn't available, so the effort estimates are against this brief's stated requirements.

---

## Appendix: forks identical to upstream or behind it (86)

All branch heads point at an upstream commit (`5ddd6d1` main, or an older tag), so there's no divergence:

joshcohen-ai, henri-edh, XAngeling, wjpeters, ttb, ThickLine, sd-rahulk, rodrirui, optimizfx, ObadiaNgenoh, moazzzmo, miguelsalcedo01, marcinpelszyka-cpu, marcellodesales, ManojSharmaK, magaalonet-arch, klausarent, kellyg0706, jeunjetta, jedson45, ivan119, hapcheto, Fanta-Zero, envisionedbrands, duan-nguyen, diogorcamargo, digitalhustlerx, Desmondotutu, Codzart, chattala, c0debrain, Bushy-Given, beauagents, badsector666, atarim-info, Anti-theist, AmosLain, aitsamahad, aibuilder696 (`beebots---letsmodifyokay`), wisamokkeh, rajdeepbhattacharyya25-pixel, QalbeHabib, monteiroassis, mhshohel, allamiro, yedaaa22-pixel, webdevtodayjason, tongphuochung87, thomasperdana, rodctech, raynizm, rajat-ankel, mariusgavrila, luthfimasruri, IcodeNet, ianson-kazz, g3telemoon, dsc8x, cyberlife-coder, billbarsch, alredowanahmed (`Treading-bot`), aimadnesscreation-lab, veanusnathan, aiscimi-code, vixelai, vighneshanap, shariqriazz, sadegh-arfa, noondz20-cmd, mrqadeer, maliktanveerdhool, magictreesproductions, jeroennicolai, jaturapornchai, icarus215063, hostlogic1, gte861f, Flecho10, edyrod84, davidcoelhodev, bytenaija, appletechie, AbhinavDobhal, 7awii, 2bdk, p1ranha. (All are `<owner>/beebots` unless a name is given.)

## Sources

- Forks list: https://github.com/imikerussell/beebots/forks (pages 1–4). Fork code: anonymous `git ls-remote` / `git fetch` of each `https://github.com/<owner>/<repo>`.
- Freqtrade docs read from the repo: `docs/index.md` (exchange list), `docs/exchanges.md` (Kraken notes), `docs/configuration.md` (`dry_run`), and https://github.com/freqtrade/freqtrade-strategies
- Jesse: https://github.com/jesse-ai/jesse (`jesse/enums/__init__.py`, `jesse/services/auth.py`)
- Hummingbot: https://github.com/hummingbot/hummingbot (`hummingbot/connector/exchange/`)
- OctoBot: https://github.com/Drakkar-Software/OctoBot (`packages/tentacles/Services/Services_bases/gpt_service/gpt.py`)
- NautilusTrader: https://github.com/nautechsystems/nautilus_trader (`crates/adapters/`)
- FCA crypto derivatives ban: https://fca.org.uk/news/press-releases/fca-bans-sale-crypto-derivatives-retail-consumers · https://www.fca.org.uk/publication/policy/ps20-10.pdf
- New UK regime: https://www.burges-salmon.com/our-thinking/the-uk-cryptoasset-regulatory-regime-where-we-are-now/ · https://www.bakermckenzie.com/en/insight/publications/2026/02/united-kingdom-new-cryptoassets-regime-published · https://www.gherson.com/blog/uk-cryptoasset-regulation-2026-legal-update/ · https://paymentexpert.com/2026/09/17/fca-crypto-regime-guidance-firms/
- Kraken: https://www.kraken.com/gb/legal/disclosures · https://docs.kraken.com/exchange/guides/rest/api-keys · https://support.kraken.com/hc/en-us/articles/360024809011-API-Testing-Environment · https://www.datawallet.com/crypto/kraken-fees-explained · https://www.dollarscout.net/crypto/reviews/kraken/
- Coinbase: https://dev.coindesk.com/policy/2025/02/03/coinbase-secures-spot-on-uk-crypto-register · https://cointelegraph.com/news/coinbase-fca-uk-crypto-registration · https://brokerchooser.com/broker-reviews/coinbase-review/coinbase-fees · https://docs.cdp.coinbase.com/coinbase-app/advanced-trade-apis/sandbox · https://docs.cloud.coinbase.com/advanced-trade/docs/rest-api-scopes
- Alpaca UK: https://www.businesswire.com/news/home/20250710149866/en/Alpaca-Enters-UK-and-EU-Market-through-WealthKernel-Acquisition
