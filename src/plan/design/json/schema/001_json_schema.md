# JSON Schema Definition

## Root Object
```json
{
  "type": "object",
  "properties": {
    "metadata": { "$ref": "#/definitions/metadata" },
    "song": { "$ref": "#/definitions/song" },
    "rhyming_groups": { "type": "array", "items": { "$ref": "#/definitions/rhyming_group" } },
    "combinations": { "type": "array", "items": { "$ref": "#/definitions/combination" } },
    "active_combination_id": { "type": "string" }
  },
  "required": ["metadata", "song", "rhyming_groups", "combinations"],
  "definitions": {
    "metadata": {
      "type": "object",
      "properties": {
        "version": { "type": "string" },
        "title": { "type": "string" },
        "created": { "type": "string", "format": "date-time" },
        "modified": { "type": "string", "format": "date-time" },
        "description": { "type": "string" }
      },
      "required": ["version", "title", "created", "modified"]
    },
    "song": {
      "type": "object",
      "properties": {
        "title": { "type": "string" },
        "parts": { "type": "array", "items": { "$ref": "#/definitions/part" } }
      },
      "required": ["title", "parts"]
    },
    "part": {
      "type": "object",
      "properties": {
        "name": { "type": "string" },
        "attributes": { "type": "object", "additionalProperties": { "type": "string" } },
        "lines": { "type": "array", "items": { "$ref": "#/definitions/line" } }
      },
      "required": ["name", "lines"]
    },
    "line": {
      "type": "object",
      "properties": {
        "original_text": { "type": "string" },
        "alternatives": { "type": "array", "items": { "$ref": "#/definitions/alternative_line" } },
        "rhyming_groups": { "type": "array", "items": { "type": "string" } },
        "position": { "type": "integer" }
      },
      "required": ["original_text", "alternatives", "position"]
    },
    "alternative_line": {
      "type": "object",
      "properties": {
        "id": { "type": "string" },
        "text": { "type": "string" },
        "rhyming_score": { "type": "number" },
        "is_final": { "type": "boolean" }
      },
      "required": ["id", "text", "rhyming_score", "is_final"]
    },
    "rhyming_group": {
      "type": "object",
      "properties": {
        "group_id": { "type": "string" },
        "group_type": { "type": "string", "enum": ["END_RHYME", "INLINE_RHYME", "BEGIN_RHYME"] },
        "member_lines": {
          "type": "array",
          "items": {
            "type": "object",
            "properties": {
              "part_name": { "type": "string" },
              "line_position": { "type": "integer" }
            },
            "required": ["part_name", "line_position"]
          }
        }
      },
      "required": ["group_id", "group_type", "member_lines"]
    },
    "combination": {
      "type": "object",
      "properties": {
        "combination_id": { "type": "string" },
        "line_selections": {
          "type": "object",
          "additionalProperties": { "type": "string" }
        },
        "total_rhyming_score": { "type": "number" },
        "is_final": { "type": "boolean" }
      },
      "required": ["combination_id", "line_selections", "total_rhyming_score", "is_final"]
    }
  }
}
```