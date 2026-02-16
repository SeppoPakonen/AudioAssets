# JSON Example

## Sample Project File
```json
{
  "metadata": {
    "version": "1.0",
    "title": "Sample Song",
    "created": "2026-02-09T10:00:00Z",
    "modified": "2026-02-09T10:00:00Z",
    "description": "A sample song for testing the rhyming calculator"
  },
  "song": {
    "title": "Summer Days",
    "parts": [
      {
        "name": "Verse 1",
        "attributes": {
          "style": "singing",
          "gender": "male"
        },
        "lines": [
          {
            "original_text": "Walking down the sunny street",
            "alternatives": [
              {
                "id": "alt1",
                "text": "Strolling on the bright highway",
                "rhyming_score": 0.7,
                "is_final": false
              },
              {
                "id": "alt2",
                "text": "Marching through the summer heat",
                "rhyming_score": 0.9,
                "is_final": true
              }
            ],
            "rhyming_groups": ["end_rhyme_1"],
            "position": 0
          },
          {
            "original_text": "Feeling so refreshingly neat",
            "alternatives": [
              {
                "id": "alt3",
                "text": "Life feels so bittersweet",
                "rhyming_score": 0.8,
                "is_final": false
              },
              {
                "id": "alt4",
                "text": "Nothing could feel complete",
                "rhyming_score": 0.6,
                "is_final": false
              }
            ],
            "rhyming_groups": ["end_rhyme_1"],
            "position": 1
          }
        ]
      },
      {
        "name": "Chorus",
        "attributes": {
          "style": "singing",
          "gender": "female"
        },
        "lines": [
          {
            "original_text": "These summer days are here",
            "alternatives": [
              {
                "id": "alt5",
                "text": "These warm days disappear",
                "rhyming_score": 0.5,
                "is_final": false
              }
            ],
            "rhyming_groups": [],
            "position": 0
          }
        ]
      }
    ]
  },
  "rhyming_groups": [
    {
      "group_id": "end_rhyme_1",
      "group_type": "END_RHYME",
      "member_lines": [
        {
          "part_name": "Verse 1",
          "line_position": 0
        },
        {
          "part_name": "Verse 1",
          "line_position": 1
        }
      ]
    }
  ],
  "combinations": [
    {
      "combination_id": "combo1",
      "line_selections": {
        "Verse 1:0": "alt2",
        "Verse 1:1": "alt3",
        "Chorus:0": "alt5"
      },
      "total_rhyming_score": 2.2,
      "is_final": true
    }
  ],
  "active_combination_id": "combo1"
}
```