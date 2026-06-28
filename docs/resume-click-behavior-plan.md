# Resume Click Behavior Plan

## Goal

Add a power-user option for Continue Watching and Next Up clicks to auto-play the first available release instead of always opening the details/releases view.

## Proposed Setting

`resume/clickBehavior`

Options:

- `showReleases` default: open details, select the correct episode/movie, load releases, let the user choose.
- `autoPlayFirstRelease`: open details/load releases, then automatically play release index `0` when fresh streams arrive.

Keep same-release matching out of the first implementation. It is useful later, but inconsistent unless Miru has local release metadata for the exact item.

## UX Rules

- Default must remain `showReleases` for safe, predictable behavior.
- `autoPlayFirstRelease` is opt-in for users who trust their AIOStreams filtering/ranking.
- If no streams are found, stay on the details page and show the existing empty/no-sources state.
- If the user navigates away, searches, changes episode, or manually starts another item, cancel pending autoplay.
- Continue Watching and Next Up can share the same setting initially.

## Implementation Shape

1. Add persistent setting in `ApplicationController`:
   - Store under `resume/clickBehavior` in `QSettings`.
   - Expose QML property `resumeClickBehavior`.
   - Add setter and change signal.

2. Add Settings UI choice:
   - `Show releases`.
   - `Auto-play first release`.

3. Track pending autoplay state:
   - Pending type/id for movie or exact episode id.
   - Pending source item, if needed for resume percent.
   - Clear on navigation away, manual episode selection, search, and manual playback.

4. On resume/next-up click:
   - Existing path still opens details and calls `loadStreams()`.
   - If setting is `autoPlayFirstRelease`, mark pending autoplay for the exact stream request id.

5. On `streamsReady`:
   - Verify pending autoplay still matches the current stream request/media.
   - If streams list is non-empty, call the same playback path as clicking release index `0`.
   - For embedded player mode, reuse existing pending playback/window-id flow.
   - Clear pending autoplay after triggering or if no streams arrive.

## Risks / Edge Cases

- First release quality depends entirely on AIOStreams backend ranking/filtering.
- Late stream responses must not trigger autoplay for an old item.
- Embedded playback requires the existing delayed `playerSurface.windowId` flow.
- Trakt resume percent should still be passed through for Trakt Continue Watching entries.

## Later Option

Add `tryPreviousMiruRelease` behavior after release matching is robust:

- Try saved local stream URL if available.
- If URL fails or is missing, load releases and match by release metadata (`title`, `name`, `filename`, `description`, size/provider/badges where available).
- Fallback either to showing releases or auto-playing first release depending on a separate setting.
